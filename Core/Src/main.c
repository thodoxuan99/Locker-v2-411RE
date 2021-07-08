/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_ssl.h"
#include "app_mqtt.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char log[200];


char *projectId = "hw-smartlocker";
char *region = "asia-east1";
char *registryId = "citynow-smartlocker";
char *deviceId = "SIMCOM7600_Test";


uint8_t client_index = 0;
char clientid[200];
uint8_t clean_session = 0;
uint8_t keepAlive = 120;
char *server_address = "\"tcp://mqtt.2030.ltsapis.goog:443\"";
char *user = "\"unused\"";
char *pass = "";
char topic[100];

MQTTClient_TypeDef mqtt_client;

MESSAGE_TypeDef message;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  UART_485_Init();
  UART_DEBUG_Init();
  UART_SIM7600_Init();
  SSL_Init();
//  createJWT("hw-smartlocker", time(NULL), time(NULL)+3600);
//  char * resultt = createGCPJWT("hw-smartlocker", time(NULL), time(NULL)+3600);
//  UART_DEBUG_Transmit(resultt);
  /* USER CODE BEGIN 2 */
	sprintf(clientid,"\"projects/%s/locations/%s/registries/%s/devices/%s\"",projectId,region,registryId,deviceId);
	mqtt_client.client_index=client_index;
	mqtt_client.clientid=clientid;
	mqtt_client.server_address=server_address;
	mqtt_client.clean_session=clean_session;
	mqtt_client.keepAlive=keepAlive;
	mqtt_client.user=user;
	mqtt_client.pass=createGCPJWT("hw-smartlocker", time(NULL), time(NULL)+3600);
	sprintf(log,"iat: %d\r\n", time(NULL));
	UART_DEBUG_Transmit(log);
	sprintf(topic,"\"/devices/%s/states\"");
	message.topic=topic;
	message.payload="\"ok\"";
	message.qos = 1;
	message.retain = 0;
	message.pub_timeout = 120;
	message.dup = 0;

  MQTT_Start();

  MQTT_Accquire_Client(&mqtt_client);
//
  SSL_Typedef ssl = Get_Ssl_Config();
  MQTT_Set_Ssl(&mqtt_client, &ssl);
//
  MQTT_Connect(&mqtt_client);
  MQTT_Publish(&mqtt_client, &message);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  HAL_Delay(1000);
//	  if(UART_SIM7600_Received_Buffer_Available()){
//		  buf = UART_SIM7600_Read_Received_Buffer();
//		  UART_DEBUG_Transmit_Size(&buf, 1);
//	  }

//	  uint8_t * buf = MQTT_Get_Message_Subcribe("environment", &len);
//	  for (int var = 0; var < len; ++var) {
//		  UART_DEBUG_Transmit_Size(buf+var, 1);
//	}
//	  UART_DEBUG_Transmit_Size(buf, len);
//	  UART_DEBUG_Transmit_Size("\r\n", 2);
//	  UART_DEBUG_Transmit_Size(buf, len);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
