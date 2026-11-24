#include "sntp_service.h"

#include "nxd_dns.h"
#include "nxd_sntp_client.h"
#include "main.h"

#include <stdint.h>
#include <stdio.h>

extern RTC_HandleTypeDef hrtc;

#define NTP_UNIX_EPOCH_OFFSET  2208988800ULL
#define SNTP_DNS_WAIT_TICKS    (10U * NX_IP_PERIODIC_RATE)

static NX_DNS dns_client;
static NX_SNTP_CLIENT sntp_client;
static UCHAR dns_domain[] = "local";
static UCHAR sntp_server_host[] = "pool.ntp.org";
static UINT sntp_started;

static UINT Resolve_SNTP_Server(NX_IP *ip_ptr, ULONG *server_address_ptr);
static UINT NTP_Time_To_RTC(const NX_SNTP_TIME *ntp_time,
                            RTC_TimeTypeDef *rtc_time,
                            RTC_DateTypeDef *rtc_date);
static VOID SNTP_Time_Update_Notify(NX_SNTP_TIME_MESSAGE *time_message,
                                    NX_SNTP_TIME *local_time);

UINT UserCode_SNTP_Start(NX_IP *ip_ptr, NX_PACKET_POOL *packet_pool_ptr)
{
  UINT status;
  ULONG server_address;

  if ((ip_ptr == NX_NULL) || (packet_pool_ptr == NX_NULL))
  {
    return NX_PTR_ERROR;
  }

  if (sntp_started != 0U)
  {
    return NX_SUCCESS;
  }

  status = Resolve_SNTP_Server(ip_ptr, &server_address);
  if (status != NX_SUCCESS)
  {
    printf("SNTP server DNS lookup failed: %u\n", status);
    return status;
  }

  printf("SNTP server resolved: %lu.%lu.%lu.%lu\n",
         (server_address >> 24) & 0xFFU,
         (server_address >> 16) & 0xFFU,
         (server_address >> 8) & 0xFFU,
         server_address & 0xFFU);

  status = nx_sntp_client_create(&sntp_client,
                                ip_ptr,
                                0U,
                                packet_pool_ptr,
                                NX_NULL,
                                NX_NULL,
                                NX_NULL);
  if (status != NX_SUCCESS)
  {
    printf("SNTP client creation failed: %u\n", status);
    return status;
  }

  status = nx_sntp_client_set_time_update_notify(&sntp_client,
                                                SNTP_Time_Update_Notify);
  if (status == NX_SUCCESS)
  {
    status = nx_sntp_client_initialize_unicast(&sntp_client, server_address);
  }
  if (status == NX_SUCCESS)
  {
    status = nx_sntp_client_run_unicast(&sntp_client);
  }

  if (status != NX_SUCCESS)
  {
    printf("SNTP client startup failed: %u\n", status);
    nx_sntp_client_delete(&sntp_client);
    return status;
  }

  sntp_started = 1U;
  printf("SNTP client started; waiting for the first valid time response\n");
  return NX_SUCCESS;
}

static UINT Resolve_SNTP_Server(NX_IP *ip_ptr, ULONG *server_address_ptr)
{
  UINT status;
  UINT delete_status;
  ULONG dns_server_address;

  status = nx_ip_gateway_address_get(ip_ptr, &dns_server_address);
  if ((status != NX_SUCCESS) || (dns_server_address == 0U))
  {
    return (status == NX_SUCCESS) ? NX_NOT_SUCCESSFUL : status;
  }

  status = nx_dns_create(&dns_client, ip_ptr, dns_domain);
  if (status != NX_SUCCESS)
  {
    return status;
  }

  status = nx_dns_server_add(&dns_client, dns_server_address);
  if (status == NX_SUCCESS)
  {
    status = nx_dns_host_by_name_get(&dns_client,
                                    sntp_server_host,
                                    server_address_ptr,
                                    SNTP_DNS_WAIT_TICKS);
  }

  delete_status = nx_dns_delete(&dns_client);
  if ((status == NX_SUCCESS) && (delete_status != NX_SUCCESS))
  {
    status = delete_status;
  }

  return status;
}

static UINT NTP_Time_To_RTC(const NX_SNTP_TIME *ntp_time,
                            RTC_TimeTypeDef *rtc_time,
                            RTC_DateTypeDef *rtc_date)
{
  uint64_t unix_seconds;
  uint64_t ntp_seconds;
  uint32_t days_since_epoch;
  uint32_t seconds_today;
  int64_t shifted_days;
  int64_t era;
  int64_t year;
  uint32_t day_of_era;
  uint32_t year_of_era;
  uint32_t day_of_year;
  uint32_t month_prime;
  uint32_t month;
  uint32_t day;
  uint32_t weekday;

  if ((ntp_time == NX_NULL) || (rtc_time == NX_NULL) || (rtc_date == NX_NULL))
  {
    return NX_NOT_SUCCESSFUL;
  }

  ntp_seconds = (uint64_t)ntp_time->seconds;
  if (ntp_seconds < NTP_UNIX_EPOCH_OFFSET)
  {
    /* NTP seconds wrap in 2036; interpret low values as the next NTP era. */
    ntp_seconds += (1ULL << 32);
  }
  unix_seconds = ntp_seconds - NTP_UNIX_EPOCH_OFFSET;
  days_since_epoch = (uint32_t)(unix_seconds / 86400ULL);
  seconds_today = (uint32_t)(unix_seconds % 86400ULL);

  /* Convert days since 1970-01-01 to a Gregorian calendar date. */
  shifted_days = (int64_t)days_since_epoch + 719468;
  era = shifted_days / 146097;
  day_of_era = (uint32_t)(shifted_days - (era * 146097));
  year_of_era = (day_of_era - day_of_era / 1460U + day_of_era / 36524U -
                 day_of_era / 146096U) / 365U;
  year = (int64_t)year_of_era + (era * 400);
  day_of_year = day_of_era - (365U * year_of_era + year_of_era / 4U - year_of_era / 100U);
  month_prime = (5U * day_of_year + 2U) / 153U;
  day = day_of_year - (153U * month_prime + 2U) / 5U + 1U;
  month = (month_prime < 10U) ? (month_prime + 3U) : (month_prime - 9U);
  year += (month <= 2U) ? 1 : 0;

  /* The STM32 RTC calendar stores years from 2000 through 2099. */
  if ((year < 2000) || (year > 2099))
  {
    return NX_NOT_SUCCESSFUL;
  }

  rtc_time->Hours = (uint8_t)(seconds_today / 3600U);
  rtc_time->Minutes = (uint8_t)((seconds_today % 3600U) / 60U);
  rtc_time->Seconds = (uint8_t)(seconds_today % 60U);
  rtc_time->TimeFormat = RTC_HOURFORMAT12_AM;
  rtc_time->DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  rtc_time->StoreOperation = RTC_STOREOPERATION_RESET;

  /* 1970-01-01 was Thursday; convert Sunday=0 to HAL's Sunday=7. */
  weekday = (days_since_epoch + 4U) % 7U;
  rtc_date->WeekDay = (uint8_t)((weekday == 0U) ? RTC_WEEKDAY_SUNDAY : weekday);
  rtc_date->Month = (uint8_t)month;
  rtc_date->Date = (uint8_t)day;
  rtc_date->Year = (uint8_t)(year - 2000);

  return NX_SUCCESS;
}

static VOID SNTP_Time_Update_Notify(NX_SNTP_TIME_MESSAGE *time_message,
                                    NX_SNTP_TIME *local_time)
{
  RTC_TimeTypeDef rtc_time = {0};
  RTC_DateTypeDef rtc_date = {0};
  UINT status;

  NX_PARAMETER_NOT_USED(time_message);

  status = NTP_Time_To_RTC(local_time, &rtc_time, &rtc_date);
  if (status != NX_SUCCESS)
  {
    printf("SNTP time is outside the supported RTC calendar range\n");
    return;
  }

  if (HAL_RTC_SetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN) != HAL_OK)
  {
    printf("Failed to set RTC time from SNTP\n");
    return;
  }

  if (HAL_RTC_SetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN) != HAL_OK)
  {
    printf("Failed to set RTC date from SNTP\n");
    return;
  }

  printf("RTC synchronized to UTC: %04u-%02u-%02u %02u:%02u:%02u\n",
         (unsigned int)(2000U + rtc_date.Year),
         (unsigned int)rtc_date.Month,
         (unsigned int)rtc_date.Date,
         (unsigned int)rtc_time.Hours,
         (unsigned int)rtc_time.Minutes,
         (unsigned int)rtc_time.Seconds);
}
