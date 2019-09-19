#include "i2cifc.h"

static bool initialized = false;

int8_t I2C_Init(const I2C_MODULE_ID module) {
    if(initialized)
        return 0;

    PLIB_I2C_Disable(module);
    PLIB_I2C_BaudRateSet(module, SYS_CLK_BUS_PERIPHERAL_1, 400000);
    PLIB_I2C_StopInIdleDisable(module);
    PLIB_I2C_HighFrequencyEnable(module);
    /* Initialize interrupts */
//    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_I2C_2_MASTER);
//    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_I2C_2_MASTER);
//    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_I2C_2_ERROR);
//    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_I2C_2_ERROR);
//    PLIB_INT_VectorPrioritySet(INT_ID_0, INT_VECTOR_I2C2, INT_PRIORITY_LEVEL1);
//    PLIB_INT_VectorSubPrioritySet(INT_ID_0, INT_VECTOR_I2C2, INT_SUBPRIORITY_LEVEL0);
    PLIB_I2C_Enable(module);
    return 0;
}

void I2C_TransmitAndWait(const I2C_MODULE_ID module, const uint8_t data) {
    while(!PLIB_I2C_TransmitterIsReady(module));
    PLIB_I2C_TransmitterByteSend(module, data);
    while(!PLIB_I2C_TransmitterByteHasCompleted(module));
    while(!PLIB_I2C_TransmitterByteWasAcknowledged(module));
}
