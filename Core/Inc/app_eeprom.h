/*
 * app_eeprom.h
 *
 *  Created on: Jul 21, 2021
 *      Author: thodo
 */

#ifndef INC_APP_EEPROM_H_
#define INC_APP_EEPROM_H_
#include "main.h"

typedef struct {
	uint16_t memory_address;
	uint16_t memory_add;
	uint8_t *data;
	uint16_t size;
}EEPROM_Data;

void EEPROM_Init(void);
void EEPROM_Write_Data(EEPROM_Data *data);
void EEPROM_Read_Data(EEPROM_Data *data);
uint8_t EEPROM_Scan_Device(void);


#endif /* INC_APP_EEPROM_H_ */
