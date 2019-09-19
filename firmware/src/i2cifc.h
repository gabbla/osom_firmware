#ifndef I2CIFC_H_P5CMLASU
#define I2CIFC_H_P5CMLASU

#include <peripheral/peripheral.h>
#include "system_config.h" // for SYS_CLK_FREQ

// Both Gauge and EEprom support 400kHz


int8_t I2C_Init(const I2C_MODULE_ID module);

void I2C_TransmitAndWait(const I2C_MODULE_ID module, const uint8_t data);


#endif /* end of include guard: I2CIFC_H_P5CMLASU */
