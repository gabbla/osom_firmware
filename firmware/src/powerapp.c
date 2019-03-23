#include "powerapp.h"

POWERAPP_DATA powerappData;

void I2C_Init(const I2C_MODULE_ID module, const uint32_t freq) {
    PLIB_I2C_Disable(module);
    PLIB_I2C_SMBDisable(module);
    PLIB_I2C_HighFrequencyEnable(module);
    PLIB_I2C_StopInIdleDisable(module);
    PLIB_I2C_BaudRateSet(module, SYS_CLK_FREQ, freq);
    PLIB_I2C_Enable(module);
}

bool _i2c_write_and_check(const I2C_MODULE_ID module, const uint8_t data) {
    PLIB_I2C_TransmitterByteSend(module, data);
    while (PLIB_I2C_TransmitterIsBusy(module))
        ;
    if (!PLIB_I2C_TransmitterByteWasAcknowledged(module)) {
        asm("nop");
        return false;
    }
    return true;
}

void I2C_WriteByte(const I2C_MODULE_ID module, const uint8_t address,
                   const uint8_t data) {
    // Wait for idle bus
    while (!PLIB_I2C_BusIsIdle(module))
        ;
    // Generate start condition
    PLIB_I2C_MasterStart(module);
    // Send address TODO mask with 0 for !W
    if (!_i2c_write_and_check(module, address)) return;
    if (!_i2c_write_and_check(module, data)) return;
    PLIB_I2C_MasterStop(module);
}

bool BQ27441_GetData(const BQ27441_Command cmd, BQ27441_CALLBACK cb) {
    if (!powerappData.operationInProgress) {
        uint8_t tmp[] = { (uint8_t)cmd, (uint8_t)cmd + 1 };
        memcpy(powerappData.bqCommand, tmp, 2);
        powerappData.state = GET_VOLTAGE;
        powerappData.bqCallback = cb;
        powerappData.operationInProgress = true;
        powerappData.bqCmdId = cmd;
        return true;
    }
    return false;
}

/*******************************************************************************
  Function:
    void POWERAPP_Initialize ( void )

  Remarks:
    See prototype in powerapp.h.
 */

void POWERAPP_Initialize(void) { powerappData.state = POWERAPP_STATE_INIT; }

/******************************************************************************
  Function:
    void POWERAPP_Tasks ( void )

  Remarks:
    See prototype in powerapp.h.
 */

uint8_t voltage[] = {4, 5, 0};
uint8_t received[] = {0, 0, 0};

void POWERAPP_Tasks(void) {
    /* Check the application's current state. */
    switch (powerappData.state) {
        /* Application's initial state. */
        case POWERAPP_STATE_INIT: {
            bool appInitialized = true;
            powerappData.gauge =
                DRV_I2C_Open(DRV_I2C_INDEX_0, DRV_IO_INTENT_READWRITE);
            if (powerappData.gauge == DRV_HANDLE_INVALID) {
                ERROR("Gauge I2C handler is invalid");
                appInitialized = false;
            }

            asm("nop");
            if (appInitialized) {
                powerappData.state = POWERAPP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case POWERAPP_STATE_SERVICE_TASKS: {
            break;
        }

        case POWERAPP_STATE_IDLE: {
            break;
        }

        case GET_VOLTAGE: {
            powerappData.hBuff = DRV_I2C_TransmitThenReceive(
                powerappData.gauge, 0xAA, powerappData.bqCommand, 2,
                powerappData.bqReply, 2, NULL);
            if (!powerappData.hBuff) {
                powerappData.state = POWERAPP_STATE_ERROR;
                break;
            }
            powerappData.state = POWERAPP_STATE_WAIT;
            break;
        }

        case POWERAPP_STATE_WAIT: {
            DRV_I2C_BUFFER_EVENT status = DRV_I2C_TransferStatusGet(
                powerappData.gauge, powerappData.hBuff);
            if (status == DRV_I2C_BUFFER_EVENT_COMPLETE) {
                powerappData.operationInProgress = false;
                powerappData.bqCallback(powerappData.bqCmdId,
                                        powerappData.bqReply, 2);
                powerappData.state = POWERAPP_STATE_IDLE;
            } else if (status == DRV_I2C_BUFFER_EVENT_ERROR) {
                powerappData.state = POWERAPP_STATE_ERROR;
            }

            break;
        }

        case POWERAPP_STATE_ERROR: {
            powerappData.bqCallback(powerappData.bqCmdId, NULL, 0);
            powerappData.operationInProgress = false;
            break;
        }

        /* The default state should never be executed. */
        default: { break; }
    }
}
