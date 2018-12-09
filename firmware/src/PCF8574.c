/*
 * PCF8574.c
 *
 *  Created on: 04 nov 2018
 *      Author: gabbla
 */

#include "PCF8574.h"

void PCF_BankWrite(I2CDevice *dev, uint8_t bank) {
	DRV_HANDLE handle = I2CDevice_GetHandle(dev);
	uint16_t dev_address = I2CDevice_GetWriteAddress(dev);
	uint8_t buff[] = { 0x00, 0x06, 1, 2, 3, 4, 5, 6, 7 };

	DRV_I2C_BUFFER_HANDLE res = DRV_I2C_Transmit(handle, 0x0040, &buff[0], 9, NULL);

	if(res == DRV_I2C_BUFFER_HANDLE_INVALID){
		// Error

	}
}
