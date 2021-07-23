/*
 * app_gpio.c
 *
 *  Created on: Nov 22, 2019
 *      Author: VAIO
 */
#include "main.h"

void GPIO_Init(void);
void DipSwitch_Init(void);
void LED_Init(void);
void Buzzer_Init(void);


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void GPIO_Init(void)
{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

//	/*Configure GPIO pin Output Level */
//	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
//

	DipSwitch_Init();
	LED_Init();
	Buzzer_Init();
}

void DipSwitch_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = DIPSWITCH_PIN[0];
	HAL_GPIO_Init(DIPSWITCH_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = DIPSWITCH_PIN[1];
	HAL_GPIO_Init(DIPSWITCH_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = DIPSWITCH_PIN[2];
	HAL_GPIO_Init(DIPSWITCH_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = DIPSWITCH_PIN[3];
	HAL_GPIO_Init(DIPSWITCH_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = DIPSWITCH_PIN[4];
	HAL_GPIO_Init(DIPSWITCH_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = DIPSWITCH_PIN[5];
	HAL_GPIO_Init(DIPSWITCH_PORT, &GPIO_InitStruct);
}


void LED_Init(void){

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = LED_STATUS_PIN;
	HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LED_RX_PIN;
	HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LED_TX_PIN;
	HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
}


void Buzzer_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	GPIO_InitStruct.Pin = BUZZER_PIN;
	HAL_GPIO_Init(BUZZER_PORT, &GPIO_InitStruct);
}


uint8_t Get_Dipswitch_ID(void){
	uint8_t dipswitch_id=0x00;
	for(uint8_t index =0;index <6;index++){
		dipswitch_id |= HAL_GPIO_ReadPin(DIPSWITCH_PORT, DIPSWITCH_PIN[index])<<index;
	}
	return dipswitch_id;
}

void Turn_On_Buzzer(void){
	HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, SET);
}

void Turn_Off_Buzzer(void){
	HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, RESET);
}





