/*
 * app_uart.h
 *
 *  Created on: Jun 23, 2021
 *      Author: thodo
 */

#ifndef INC_APP_UART_H_
#define INC_APP_UART_H_

#include "main.h"

typedef enum {
	AT_OK=0,
	AT_ERROR,
	AT_INPUT
}AT_Result;


typedef struct {
	uint8_t * result;
	AT_Result at_result;
}Result_TypeDef;

// UART for SIM7600 - UART1
void UART_SIM7600_Init(void);
void UART_SIM7600_Transmit(uint8_t *buffer);
void UART_SIM7600_Transmit_Size(uint8_t *buffer , uint8_t buffer_size);
uint8_t UART_SIM7600_Received_Buffer_Available(void);
uint8_t UART_SIM7600_Read_Received_Buffer(void);

// UART for 485 Board Control - UART2
void UART_485_Init(void);
void UART_485_Transmit(uint8_t *buffer);
void UART_485_Transmit_Size(uint8_t *buffer , uint8_t buffer_len);
uint8_t UART_485_Received_Buffer_Available(void);
uint8_t UART_485_Read_Received_Buffer(void);


// UART for DEBUG - UART3
void UART_DEBUG_Init(void);
void UART_DEBUG_Transmit(uint8_t *buffer);
void UART_DEBUG_Transmit_Size(uint8_t *buffer ,uint8_t buffer_size);



HAL_StatusTypeDef Custom_UART_Receive_IT(UART_HandleTypeDef *huart);


// Get buffer content with expect_result for stop
uint8_t* Result_From_Buffer(char * expect_result);
void Wait_For_Respone();
Result_TypeDef Get_AT_Result();


//Get pattern in Buffer
uint8_t* Get_Uart_Pattern(char * pattern ,uint8_t* match_length);

//Clear buffer after receive from 1 respone AT command
void Clear_Buffer(void);
// Time out for RxCallBack
void RxTimeOut();

#endif /* INC_APP_UART_H_ */
