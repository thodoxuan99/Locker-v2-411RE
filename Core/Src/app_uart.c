/*
 * app_uart.c
 *
 *  Created on: Jun 23, 2021
 *      Author: thodo
 */

#define DEBUG	1

#include "app_uart.h"
#include "stdio.h"
#include "stdbool.h";

char temp_array[RXBUFFERSIZE];

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;

uint8_t temp;
/* Buffer used for reception */
uint8_t aUART_RxBuffer[RXBUFFERSIZE];
uint8_t receiveBufferIndexHead = 0;
uint8_t receiveBufferIndexTail = 0;

// Buffer for get result and timeout flag
uint8_t timeout_flag;
uint8_t atResult[RXBUFFERSIZE];

//AT result
Result_TypeDef _result;

__IO ITStatus UartTransmitReady = SET;
__IO ITStatus UartReceiveReady = RESET;


void UART_SIM7600_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  Setup_Receive_IT();
  /* USER CODE END USART1_Init 2 */

}
void UART_SIM7600_Transmit(uint8_t *buffer){
	if(strlen(buffer) == 0) {
		return;
	} else {
		if (DEBUG){
			while(!UartTransmitReady);
			UART_DEBUG_Transmit(buffer);
		}
		while(!UartTransmitReady);
		if(HAL_UART_Transmit_IT(&huart1, (uint8_t*)buffer, strlen(buffer))!= HAL_OK){
			Error_Handler();
		}
		UartTransmitReady = RESET;
	}
	return;
}

void UART_SIM7600_Transmit_Size(uint8_t *buffer , uint8_t buffer_size){
	if(buffer_size == 0) {
		return;
	} else {
		if (DEBUG){
			while(!UartTransmitReady);
			UART_DEBUG_Transmit_Size(buffer,buffer_size);
		}
		while(!UartTransmitReady);
		if(HAL_UART_Transmit_IT(&huart1, (uint8_t*)buffer, buffer_size)!= HAL_OK){
			Error_Handler();
		}
		UartTransmitReady = RESET;
	}
	return;
}

uint8_t UART_SIM7600_Received_Buffer_Available(void){
	if(receiveBufferIndexTail != receiveBufferIndexHead){
		return 1;
	} else {
		return 0;
	}
}

uint8_t UART_SIM7600_Read_Received_Buffer(void){
	if(receiveBufferIndexTail == receiveBufferIndexHead) return 0xff;
	uint8_t ch = aUART_RxBuffer[receiveBufferIndexTail];
	receiveBufferIndexTail = (receiveBufferIndexTail + 1) % RXBUFFERSIZE;
	return ch;
}


/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */

void UART_485_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

void UART_485_Transmit_Size(uint8_t *buffer , uint8_t buffer_len){
	if(buffer_len == 0) {
		return;
	} else {
		while(!UartTransmitReady);
		if(HAL_UART_Transmit_IT(&huart6, (uint8_t*)buffer, buffer_len)!= HAL_OK){
			Error_Handler();
		}
		UartTransmitReady = RESET;
	}
	return;
}

uint8_t UART_485_Received_Buffer_Available(void){
	if(receiveBufferIndexTail != receiveBufferIndexHead){
		return 1;
	} else {
		return 0;
	}
}

uint8_t UART_485_Read_Received_Buffer(void){
	if(receiveBufferIndexTail == receiveBufferIndexHead) return 0xff;
	uint8_t ch = aUART_RxBuffer[receiveBufferIndexTail];
	receiveBufferIndexTail = (receiveBufferIndexTail + 1) % RXBUFFERSIZE;
	return ch;
}


/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
void UART_DEBUG_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

void UART_DEBUG_Transmit(uint8_t *buffer){
	while(!UartTransmitReady);
	if(HAL_UART_Transmit_IT(&huart2, (uint8_t*)buffer, strlen(buffer))!= HAL_OK){
		Error_Handler();
	}
	UartTransmitReady = RESET;
}

void UART_DEBUG_Transmit_Size(uint8_t *buffer ,uint8_t buffer_size){
	while(!UartTransmitReady);
	if(HAL_UART_Transmit_IT(&huart2, (uint8_t*)buffer, buffer_size)!= HAL_OK){
		Error_Handler();
	}
	UartTransmitReady = RESET;
}



void Setup_Receive_IT(void){
	if(HAL_UART_Receive_IT(&huart1, &temp, 1)!= HAL_OK){
		Error_Handler();
		return HAL_ERROR;
	}
	return HAL_OK;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART1){
		aUART_RxBuffer[receiveBufferIndexHead] = temp;
		receiveBufferIndexHead = (receiveBufferIndexHead + 1) % RXBUFFERSIZE;
		HAL_UART_Receive_IT(&huart1, &temp, 1);
	}
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle){
  /* Set transmission flag: transfer complete */
  UartTransmitReady = SET;
}

void Clear_Reiceive_Buffer(void){
	memset(aUART_RxBuffer,0,RXBUFFERSIZE);
	receiveBufferIndexHead = 0;
	receiveBufferIndexTail = 0;
}

void Clear_Temp_Buffer(void){
	memset(temp_array,0,RXBUFFERSIZE);
}

void Wait_For_Respone(){
	int index = 0;
	Clear_Temp_Buffer();
	while(1){
		if(UART_SIM7600_Received_Buffer_Available()){
			temp_array[index++]=UART_SIM7600_Read_Received_Buffer();
//			UART_DEBUG_Transmit_Size(temp_array+index, 1);
			if(strstr(temp_array,"OK\r\n")!=NULL){
				UART_DEBUG_Transmit("\r\nOK\r\n");
				_result.at_result = AT_OK;
				break;
			}
			if(strstr(temp_array,"ERROR\r\n")!=NULL){
				UART_DEBUG_Transmit("\r\nERROR\r\n");
				_result.at_result = AT_ERROR;
				break;
			}
			if(strstr(temp_array,">")!=NULL){
				UART_DEBUG_Transmit("\r\nINPUT\r\n");
				_result.at_result = AT_INPUT;
				break;
			}
		}
	}
//	UART_DEBUG_Transmit_Size(aUART_RxBuffer, RXBUFFERSIZE);
	return;
}

uint8_t* Get_Uart_Pattern(char * pattern ,uint8_t* match_length ){

	/* Check if the regex matches the text: */
	uint8_t payload_length = 0;
	int index=0;
	bool matched = false;
	char *point_to_pattern;
	char *payload_pattern = "+CMQTTRXPAYLOAD: ";
	char *end_pattern = "+CMQTTRXEND";
	Clear_Temp_Buffer();
	while(1){
		if(UART_SIM7600_Received_Buffer_Available()){
			temp_array[index++]=UART_SIM7600_Read_Received_Buffer();
//			UART_DEBUG_Transmit_Size(temp_array+index-1, 1);
			char * temp_point_payload = strstr(temp_array,payload_pattern);
			if(temp_point_payload!=NULL && !matched){
				point_to_pattern = temp_point_payload;
				matched = true;
			}

			char * temp_point_end = strstr(temp_array,end_pattern);
			if(temp_point_end!=NULL){
				char temp_char;
				int point_index=0;
//				UART_DEBUG_Transmit("\r\nEND\r\n");
				while(1){
					temp_char = point_to_pattern[strlen(payload_pattern)+2+point_index];
					UART_DEBUG_Transmit_Size(&temp_char,1);
					if(temp_char=='\r'){
//						UART_DEBUG_Transmit("\r\nBREAK\r\n");
						break;
					}
					payload_length = (uint8_t)temp_char-48;
					point_index++;
				}
				*match_length = payload_length;
				UART_DEBUG_Transmit_Size(match_length,1);
//				UART_DEBUG_Transmit("\r\nRETURN\r\n");
				return point_to_pattern+strlen(payload_pattern)+2+point_index+2;
			}
		}
	}
}


Result_TypeDef Get_AT_Result(){
	return _result;
}

