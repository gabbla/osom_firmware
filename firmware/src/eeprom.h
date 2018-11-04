/*
 * eeprom.h
 *
 *  Created on: 04 nov 2018
 *      Author: gabbla
 */

#ifndef FIRMWARE_SRC_EEPROM_H_
#define FIRMWARE_SRC_EEPROM_H_

#include "driver/i2c/drv_i2c.h"
#include "i2c_device.h"

void EEPROM_Write(const I2CDevice *dev, const uint16_t address, const uint8_t data);


#endif /* FIRMWARE_SRC_EEPROM_H_ */
