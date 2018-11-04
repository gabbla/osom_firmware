/*
 * PCF8574.h
 *
 *  Created on: 04 nov 2018
 *      Author: gabbla
 */

#ifndef FIRMWARE_PCF8574_H_
#define FIRMWARE_PCF8574_H_

#include "driver/i2c/drv_i2c.h"
#include "i2c_device.h"

void PCF_BankWrite(I2CDevice *dev, uint8_t bank);


#endif /* FIRMWARE_PCF8574_H_ */
