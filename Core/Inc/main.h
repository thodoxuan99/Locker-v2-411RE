/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

// GPIO Constant

//Led GPIO
#define LED_PORT					GPIOA
#define LED_STATUS_PIN				GPIO_PIN_1
#define LED_RX_PIN					GPIO_PIN_2
#define LED_TX_PIN					GPIO_PIN_3

//Buzzer GPIO
#define BUZZER_PORT					GPIOA
#define BUZZER_PIN					GPIO_PIN_4

//Dipswitch GPIO
#define DIPSWITCH_PORT				GPIOB
#define DIPSWITCH_PIN				(uint16_t[]){GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_4,GPIO_PIN_5}


/*UART Constant*/

//Max buffer for RX
#define RXBUFFERSIZE 255


//UART Simcom7600 Constant
#define UART_SIMCOM_INSTANCE		USART1
#define UART_DEBUG_INSTANCE			USART2
#define UART_485_INSTANCE			USART6


/*I2C Constant*/

//EEPROM I2C
#define I2C_EEPROM_INSTANCE			I2C1


/*Timer Constant*/
#define TIMER_INSTANCE				TIM3



/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
