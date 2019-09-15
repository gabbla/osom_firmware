#include "eeprom.h"

static I2C_MODULE_ID _module;
static uint8_t _address;


int8_t EEPROM_Init(const I2C_MODULE_ID module, const uint8_t address) {
    _module = module;
    _address = address;
    return I2C_Init(_module);
}

void EEPROM_Write(const uint16_t address, const uint8_t data) {
    PLIB_I2C_MasterStart(_module);
    I2C_TransmitAndWait(_module, _address & 0xFE);
    I2C_TransmitAndWait(_module, address);
    I2C_TransmitAndWait(_module, data);
    PLIB_I2C_MasterStop(_module);
}

