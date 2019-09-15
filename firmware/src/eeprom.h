#ifndef FIRMWARE_SRC_EEPROM_H_
#define FIRMWARE_SRC_EEPROM_H_

#include "i2cifc.h"

int8_t EEPROM_Init(const I2C_MODULE_ID module, const uint8_t address);

void EEPROM_Write(const uint16_t address, const uint8_t data);


#endif /* FIRMWARE_SRC_EEPROM_H_ */
