#include "nrfapp.h"

NRFAPP_DATA nrfappData;

/*******************************************************************************
  Function:
    void NRFAPP_Initialize ( void )

  Remarks:
    See prototype in nrfapp.h.
 */

void NRFAPP_Initialize(void) {
    /* Place the App state machine in its initial state. */
    nrfappData.state = NRFAPP_STATE_INIT;
}

bool configureSPI() {
    PLIB_SPI_Disable(SPI_ID_1);
    PLIB_SPI_MasterEnable(SPI_ID_1);
    PLIB_SPI_BufferClear(SPI_ID_1);
    PLIB_SPI_StopInIdleDisable(SPI_ID_1);
    PLIB_SPI_CommunicationWidthSelect(SPI_ID_1, SPI_COMMUNICATION_WIDTH_8BITS);
    PLIB_SPI_InputSamplePhaseSelect(SPI_ID_1,
                                    SPI_INPUT_SAMPLING_PHASE_IN_MIDDLE);
    PLIB_SPI_ClockPolaritySelect(SPI_ID_1,
                                 SPI_CLOCK_POLARITY_IDLE_LOW);  // CKP 0
    PLIB_SPI_OutputDataPhaseSelect(
        SPI_ID_1, SPI_OUTPUT_DATA_PHASE_ON_ACTIVE_TO_IDLE_CLOCK);  // CKE 1
    PLIB_SPI_BaudRateSet(SPI_ID_1, SYS_CLK_BUS_PERIPHERAL_1, 1000000);
    //    PLIB_SPI_PinEnable(
    //        SPI_ID_1, SPI_PIN_SLAVE_SELECT | SPI_PIN_DATA_OUT |
    //        SPI_PIN_DATA_IN);
    PLIB_SPI_FramedCommunicationDisable(SPI_ID_1);
    PLIB_SPI_FIFOEnable(SPI_ID_1);
    PLIB_SPI_Enable(SPI_ID_1);

    // PLIB_SPI_FIFOInterruptModeSelect(SPI_ID_1,
    // SPI_FIFO_INTERRUPT_WHEN_TRANSMIT_BUFFER_IS_COMPLETELY_EMPTY);

    spi_idle();
    NRF_CEOff();  // TODO
    return true;
}

void NRFAPP_Tasks(void) {
    /* Check the application's current state. */
    switch (nrfappData.state) {
            /* Application's initial state. */
        case NRFAPP_STATE_INIT:
        {
            if (configureSPI()) {
                INFO("NRF App started!");
                nrfappData.gpTimer = SYS_TMR_DelayMS(1000);
                nrfappData.state = NRFAPP_STATE_CONFIG;
            } else {
                ERROR("Failed to initialize NRF App!");
            }
            break;
        }

        case NRFAPP_STATE_CONFIG:
        {
            if (!SYS_TMR_DelayStatusGet(nrfappData.gpTimer)) break;
            volatile NRF_Status sts;
            volatile size_t kkk = sizeof (NRF_Status);
            sts = NRF_Initialize();
            asm("nop");
            break;
        }

            /* TODO: implement your application state machine.*/

            /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}
