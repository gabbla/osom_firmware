/*
 * eeprom.c
 *
 *  Created on: 04 nov 2018
 *      Author: gabbla
 */

#include "eeprom.h"

void EEPROM_Write(const I2CDevice *dev, const uint16_t address, const uint8_t data) {
	DRV_HANDLE handle = I2CDevice_GetHandle(dev);
	uint16_t dev_address = I2CDevice_GetWriteAddress(dev);
	//DRV_I2C_MasterStart(handle);
	uint8_t buff[] = {0xF0, 0x55};
	DRV_I2C_Transmit(handle, 0x00A0, (void*)buff, 2, NULL);
	//DRV_I2C_MasterStop(handle);
}

