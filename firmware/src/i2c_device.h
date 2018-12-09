/*
 * i2c_device.h
 *
 *  Created on: 04 nov 2018
 *      Author: gabbla
 */

#ifndef FIRMWARE_SRC_I2C_DEVICE_H_
#define FIRMWARE_SRC_I2C_DEVICE_H_

#include "driver/i2c/drv_i2c.h"

typedef struct _i2cdev {
	DRV_HANDLE i2cHandle;
	uint16_t address;
}I2CDevice;

DRV_HANDLE I2CDevice_GetHandle(const I2CDevice *dev) {
	return dev->i2cHandle;
}
uint16_t I2CDevice_GetWriteAddress(const I2CDevice *dev) {
	return dev->address; // TODO
}
uint16_t I2CDevice_GetReadAddress(const I2CDevice *dev) {
	return dev->address; // TODO
}

#endif /* FIRMWARE_SRC_I2C_DEVICE_H_ */
