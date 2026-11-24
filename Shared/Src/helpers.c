#include "helpers.h"
#include "main.h"




int log_blackbox(error_codes_t error_code)
{
	uint8_t code = (uint8_t)error_code;
	uint8_t len = 0;
	  // Send the message to the black box via UART4

	if(len == HAL_UART_Transmit(&huart4, &code, 1, HAL_MAX_DELAY))
		return len;
	return -1; // Return -1 if transmission fails
}
