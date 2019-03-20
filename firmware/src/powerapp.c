#include "powerapp.h"

POWERAPP_DATA powerappData;

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

uint8_t voltage[] = {4, 5};
            uint8_t received[2];

void POWERAPP_Tasks(void) {
    /* Check the application's current state. */
    switch (powerappData.state) {
        /* Application's initial state. */
        case POWERAPP_STATE_INIT: {
            bool appInitialized = true;
            powerappData.gauge =
                DRV_I2C_Open(DRV_I2C_INDEX_0,
                             DRV_IO_INTENT_BLOCKING | DRV_IO_INTENT_READWRITE);
            if(powerappData.gauge == DRV_HANDLE_INVALID) {
                ERROR("Gauge I2C handler is invalid");
                appInitialized = false;
            }
            
            DRV_I2C_TransmitThenReceive(powerappData.gauge, GAUGE_ADDRESS,
                                        voltage, 2, received, 2, NULL);
            asm("nop");
            if (appInitialized) {
                powerappData.state = POWERAPP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case POWERAPP_STATE_SERVICE_TASKS: {
            break;
        }

        /* The default state should never be executed. */
        default: { break; }
    }
}
