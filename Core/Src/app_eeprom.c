/*
 * app_eeprom.c
 *
 *  Created on: Jul 21, 2021
 *      Author: thodo
 */
#include "app_eeprom.h"

I2C_HandleTypeDef hi2c1;

// Default value is 0xF, not device scanned
uint8_t EEPROM_I2C_ADDRESS = 0xFF;

//Instance for EEPROM read Data

EEPROM_Data data1={
		.memory_address = 0x0000,
		.memory_add		= 16,
		.data=(uint8_t*)"data1",
		.size = 10
};


EEPROM_Data data2={
		.memory_address = 0x0000,
		.memory_add		= 16,
		.data=(uint8_t*)"data2",
		.size = 10
};

HAL_StatusTypeDef ret;

char* log[20];

void EEPROM_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */
}
FlagStatus EEPROM_Check_Available_Dev(void){
	if(EEPROM_I2C_ADDRESS!=0xFF){
		return RESET;
	}
	else {
		return SET;
	}
}

void EEPROM_Write_Data(EEPROM_Data *data){
	ret = HAL_I2C_Mem_Write(&hi2c1,EEPROM_I2C_ADDRESS , data->memory_address, data->memory_add, data->data, data->size, 0xFFFF);
	if(ret!=HAL_OK){
		UART_DEBUG_Transmit("Write Data to EEPROM Failed");
	}
	else{
		UART_DEBUG_Transmit("Write Data to EEPROM Success");
	}
	return;
}


void EEPROM_Read_Data(EEPROM_Data *data){
	if(!EEPROM_Check_Available_Dev()){
		return;
	}
	ret = HAL_I2C_Mem_Read(&hi2c1,EEPROM_I2C_ADDRESS , data->memory_address, data->memory_add, data->data, data->size, 0xFFFF);
	if(ret!=HAL_OK){
		UART_DEBUG_Transmit("Read Data to EEPROM Failed");
	}
	else{
		UART_DEBUG_Transmit("Read Data to EEPROM Success");
	}
	return;
}
void EEPROM_Scan_Device(void){
	for (int DevAddress = 0; DevAddress < 128; ++DevAddress) {
		ret = HAL_I2C_IsDeviceReady(&hi2c1, DevAddress, 2, 2);
		if(ret==HAL_OK){
			EEPROM_I2C_ADDRESS = DevAddress;
			sprintf(log,"Device I2C Address:%d\r\n",DevAddress);
			UART_DEBUG_Transmit(log);
			return;
		}
		else{
			UART_DEBUG_Transmit(".");
		}
	}
	sprintf(log,"Cannot find I2C Device\r\n");
	UART_DEBUG_Transmit(log);
	return;
}
