/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

#include "stm32h7xx_nucleo.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32h7xx_hal.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
extern uint32_t events;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void MX_UART4_Init(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MOTOR_1_1_Pin GPIO_PIN_9
#define MOTOR_1_1_GPIO_Port GPIOE
#define MOTOR_1_2_Pin GPIO_PIN_11
#define MOTOR_1_2_GPIO_Port GPIOE
#define MOTOR_2_1_Pin GPIO_PIN_13
#define MOTOR_2_1_GPIO_Port GPIOE
#define MOTOR_2_2_Pin GPIO_PIN_14
#define MOTOR_2_2_GPIO_Port GPIOE
#define SERVO_1_Pin GPIO_PIN_12
#define SERVO_1_GPIO_Port GPIOD
#define SERVO_2_Pin GPIO_PIN_13
#define SERVO_2_GPIO_Port GPIOD
#define SERVO_3_Pin GPIO_PIN_14
#define SERVO_3_GPIO_Port GPIOD
#define SERVO_4_Pin GPIO_PIN_15
#define SERVO_4_GPIO_Port GPIOD
#define MOTOR_3_1_Pin GPIO_PIN_6
#define MOTOR_3_1_GPIO_Port GPIOC
#define MOTOR_3_2_Pin GPIO_PIN_7
#define MOTOR_3_2_GPIO_Port GPIOC
#define MOTOR_4_1_Pin GPIO_PIN_8
#define MOTOR_4_1_GPIO_Port GPIOC
#define MOTOR_4_2_Pin GPIO_PIN_9
#define MOTOR_4_2_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */
#define ALARM_OUT_Pin GPIO_PIN_14
#define ALARM_OUT_GPIO_Port GPIOF

#define EVT_TIM7_UPDATE 0x00000001U
#define EVT_HSEM_RELEASE 0x00000002U
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
