/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_netxduo.c
  * @author  MCD Application Team
  * @brief   NetXDuo applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* USER CODE END Header */

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* Includes ------------------------------------------------------------------*/
#include "app_netxduo.h"

/* Private includes ----------------------------------------------------------*/
#include "nxd_dhcp_client.h"
/* USER CODE BEGIN Includes */
#include "main.h"
#include "sntp_service.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define UDP_SERVER_PORT          8000U
#define UDP_SOCKET_QUEUE_DEPTH   8U
#define UDP_TEST_DESTINATION     IP_ADDRESS(192, 168, 1, 174)
#define UDP_TEST_DESTINATION_PORT 58898U

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TX_THREAD      NxAppThread;
NX_PACKET_POOL NxAppPool;
NX_IP          NetXDuoEthIpInstance;
TX_SEMAPHORE   DHCPSemaphore;
NX_DHCP        DHCPClient;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static VOID nx_app_thread_entry (ULONG thread_input);
static VOID ip_address_change_notify_callback(NX_IP *ip_instance, VOID *ptr);
/* USER CODE BEGIN PFP */
static inline void Printf_IP_Address(ULONG ip_address);
/* USER CODE END PFP */

/**
  * @brief  Application NetXDuo Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT MX_NetXDuo_Init(VOID *memory_ptr)
{
  UINT ret = NX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;
  CHAR *pointer;

  /* USER CODE BEGIN MX_NetXDuo_MEM_POOL */
  /* USER CODE END MX_NetXDuo_MEM_POOL */

  /* USER CODE BEGIN 0 */

  /* USER CODE END 0 */

  /* Initialize the NetXDuo system. */
  nx_system_initialize();

    /* Allocate the memory for packet_pool.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, NX_APP_PACKET_POOL_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the Packet pool to be used for packet allocation,
   * If extra NX_PACKET are to be used the NX_APP_PACKET_POOL_SIZE should be increased
   */
  ret = nx_packet_pool_create(&NxAppPool, "NetXDuo App Pool", DEFAULT_PAYLOAD_SIZE, pointer, NX_APP_PACKET_POOL_SIZE);

  if (ret != NX_SUCCESS)
  {
    return NX_POOL_ERROR;
  }

    /* Allocate the memory for Ip_Instance */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, Nx_IP_INSTANCE_THREAD_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

   /* Create the main NX_IP instance */
  ret = nx_ip_create(&NetXDuoEthIpInstance, "NetX Ip instance", NX_APP_DEFAULT_IP_ADDRESS, NX_APP_DEFAULT_NET_MASK, &NxAppPool, nx_stm32_eth_driver,
                     pointer, Nx_IP_INSTANCE_THREAD_SIZE, NX_APP_INSTANCE_PRIORITY);

  if (ret != NX_SUCCESS)
  {
    return NX_NOT_SUCCESSFUL;
  }

    /* Allocate the memory for ARP */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, DEFAULT_ARP_CACHE_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Enable the ARP protocol and provide the ARP cache size for the IP instance */

  /* USER CODE BEGIN ARP_Protocol_Initialization */

  /* USER CODE END ARP_Protocol_Initialization */

  ret = nx_arp_enable(&NetXDuoEthIpInstance, (VOID *)pointer, DEFAULT_ARP_CACHE_SIZE);

  if (ret != NX_SUCCESS)
  {
    return NX_NOT_SUCCESSFUL;
  }

  /* Enable the ICMP */

  /* USER CODE BEGIN ICMP_Protocol_Initialization */

  /* USER CODE END ICMP_Protocol_Initialization */

  ret = nx_icmp_enable(&NetXDuoEthIpInstance);

  if (ret != NX_SUCCESS)
  {
    return NX_NOT_SUCCESSFUL;
  }

  /* Enable TCP Protocol */

  /* USER CODE BEGIN TCP_Protocol_Initialization */

  /* USER CODE END TCP_Protocol_Initialization */

  ret = nx_tcp_enable(&NetXDuoEthIpInstance);

  if (ret != NX_SUCCESS)
  {
    return NX_NOT_SUCCESSFUL;
  }

  /* Enable the UDP protocol required for  DHCP communication */

  /* USER CODE BEGIN UDP_Protocol_Initialization */

  /* USER CODE END UDP_Protocol_Initialization */

  ret = nx_udp_enable(&NetXDuoEthIpInstance);

  if (ret != NX_SUCCESS)
  {
    return NX_NOT_SUCCESSFUL;
  }

   /* Allocate the memory for main thread   */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, NX_APP_THREAD_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the main thread */
  ret = tx_thread_create(&NxAppThread, "NetXDuo App thread", nx_app_thread_entry , 0, pointer, NX_APP_THREAD_STACK_SIZE,
                         NX_APP_THREAD_PRIORITY, NX_APP_THREAD_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);

  if (ret != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* Create the DHCP client */

  /* USER CODE BEGIN DHCP_Protocol_Initialization */

  /* USER CODE END DHCP_Protocol_Initialization */

  ret = nx_dhcp_create(&DHCPClient, &NetXDuoEthIpInstance, "DHCP Client");

  if (ret != NX_SUCCESS)
  {
    return NX_DHCP_ERROR;
  }

  /* set DHCP notification callback  */
  tx_semaphore_create(&DHCPSemaphore, "DHCP Semaphore", 0);

  /* USER CODE BEGIN MX_NetXDuo_Init */
  /* USER CODE END MX_NetXDuo_Init */

  return ret;
}

/**
* @brief  ip address change callback.
* @param ip_instance: NX_IP instance
* @param ptr: user data
* @retval none
*/
static VOID ip_address_change_notify_callback(NX_IP *ip_instance, VOID *ptr)
{
  /* USER CODE BEGIN ip_address_change_notify_callback */
  NX_PARAMETER_NOT_USED(ip_instance);
  NX_PARAMETER_NOT_USED(ptr);

  /* Wake the app thread; it checks the current address after the notification. */
  tx_semaphore_put(&DHCPSemaphore);
  /* USER CODE END ip_address_change_notify_callback */
}

/**
* @brief  Main thread entry.
* @param thread_input: ULONG user argument used by the thread entry
* @retval none
*/
static VOID nx_app_thread_entry (ULONG thread_input)
{
  /* USER CODE BEGIN Nx_App_Thread_Entry 0 */
  /* USER CODE END Nx_App_Thread_Entry 0 */

  UINT ret = NX_SUCCESS;

  /* USER CODE BEGIN Nx_App_Thread_Entry 1 */
  /*
   * CubeMX's generated semaphore wait below is unconditional. Seed the
   * semaphore so the app can poll the IP address after DHCP is started,
   * instead of depending on the address-change notification to wake it.
   */
  ret = tx_semaphore_put(&DHCPSemaphore);
  if (ret != TX_SUCCESS)
  {
    printf("Could not release DHCP startup wait: %u\n", ret);
    return;
  }
  /* USER CODE END Nx_App_Thread_Entry 1 */

  /* register the IP address change callback */
  ret = nx_ip_address_change_notify(&NetXDuoEthIpInstance, ip_address_change_notify_callback, NULL);
  if (ret != NX_SUCCESS)
  {
    /* USER CODE BEGIN IP address change callback error */
    printf("IP address callback registration failed: %u\n", ret);
    return;
    /* USER CODE END IP address change callback error */
  }

  /* start the DHCP client */
  ret = nx_dhcp_start(&DHCPClient);
  if (ret != NX_SUCCESS)
  {
    /* USER CODE BEGIN DHCP client start error */
    printf("DHCP client start failed: %u\n", ret);
    return;
    /* USER CODE END DHCP client start error */
  }
  printf("Looking for DHCP server ..\n");
  /* wait until an IP address is ready */
  if(tx_semaphore_get(&DHCPSemaphore, TX_WAIT_FOREVER) != TX_SUCCESS)
  {
    /* USER CODE BEGIN DHCPSemaphore get error */
    printf("Failed waiting for DHCP address notification\n");
    return;
    /* USER CODE END DHCPSemaphore get error */
  }

  /* USER CODE BEGIN Nx_App_Thread_Entry 2 */
  NX_UDP_SOCKET udp_socket;
  ULONG ip_address;
  ULONG network_mask;
  NX_PACKET *packet;
  ULONG packet_length = 0U;
  ULONG dhcp_wait_seconds = 0U;

  printf("Polling for DHCP address...\n");
  for (;;)
  {
    ret = nx_ip_address_get(&NetXDuoEthIpInstance, &ip_address, &network_mask);
    if (ret != NX_SUCCESS)
    {
      printf("Could not read DHCP address: %u\n", ret);
      return;
    }

    if (ip_address != IP_ADDRESS(0, 0, 0, 0))
    {
      break;
    }

    tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND);
    dhcp_wait_seconds++;
    if ((dhcp_wait_seconds % 5U) == 0U)
    {
      printf("Still waiting for DHCP lease (%lu s)\n", dhcp_wait_seconds);
    }
  }

  printf("DHCP address acquired: ");
  Printf_IP_Address(ip_address);

  ret = UserCode_SNTP_Start(&NetXDuoEthIpInstance, &NxAppPool);
  if (ret != NX_SUCCESS)
  {
    printf("SNTP startup failed; RTC will remain unsynchronized: %u\n", ret);
  }

  ret = nx_udp_socket_create(&NetXDuoEthIpInstance,
                             &udp_socket,
                             "UDP Server Socket",
                             NX_IP_NORMAL,
                             NX_FRAGMENT_OKAY,
                             NX_IP_TIME_TO_LIVE,
                             UDP_SOCKET_QUEUE_DEPTH);
  if (ret != NX_SUCCESS)
  {
    printf("UDP socket creation failed: %u\n", ret);
    return;
  }

  ret = nx_udp_socket_bind(&udp_socket, UDP_SERVER_PORT, TX_WAIT_FOREVER);
  if (ret != NX_SUCCESS)
  {
    printf("UDP socket bind failed: %u\n", ret);
    nx_udp_socket_delete(&udp_socket);
    return;
  }

  printf("UDP server listening on port %u\n", UDP_SERVER_PORT);

  {
    static const UCHAR test_message[] = "NetX Duo UDP test from STM32";
    NX_PACKET *tx_packet = NX_NULL;

    ret = nx_packet_allocate(&NxAppPool, &tx_packet, NX_UDP_PACKET, TX_WAIT_FOREVER);
    if (ret == NX_SUCCESS)
    {
      ret = nx_packet_data_append(tx_packet,
                                  (VOID *)test_message,
                                  sizeof(test_message) - 1U,
                                  &NxAppPool,
                                  TX_WAIT_FOREVER);
      if (ret == NX_SUCCESS)
      {
        ret = nx_udp_socket_send(&udp_socket,
                                 tx_packet,
                                 UDP_TEST_DESTINATION,
                                 UDP_TEST_DESTINATION_PORT);
        if (ret == NX_SUCCESS)
        {
          tx_packet = NX_NULL; /* Ownership passes to NetX on successful send. */
        }
      }

      if (tx_packet != NX_NULL)
      {
        nx_packet_release(tx_packet);
      }
    }

    if (ret == NX_SUCCESS)
    {
      printf("One UDP test packet sent to 192.168.1.174:%u\n", UDP_TEST_DESTINATION_PORT);
    }
    else
    {
      printf("UDP test send failed: %u\n", ret);
    }
  }

  while (1)
  {
    ret = nx_udp_socket_receive(&udp_socket, &packet, TX_WAIT_FOREVER);

    if (ret != NX_SUCCESS)
    {
      printf("UDP receive failed: %u\n", ret);
      continue;
    }

    ret = nx_packet_length_get(packet, &packet_length);
    if (ret == NX_SUCCESS)
    {
      printf("UDP packet received (%lu bytes)\n", packet_length);
    }
    else
    {
      printf("UDP packet length query failed: %u\n", ret);
    }
    nx_packet_release(packet);
  }
  /* USER CODE END Nx_App_Thread_Entry 2 */

}
/* USER CODE BEGIN 2 */
static inline void Printf_IP_Address(ULONG ip_address)
{
	printf("IP Address: %lu.%lu.%lu.%lu\n",
		   (ip_address >> 24) & 0xFF,
		   (ip_address >> 16) & 0xFF,
		   (ip_address >> 8) & 0xFF,
		   ip_address & 0xFF);
}
/* USER CODE END 2 */
