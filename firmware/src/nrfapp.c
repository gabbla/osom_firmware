#include "nrfapp.h"

NRFAPP_DATA nrfappData;

void __ISR(_EXTERNAL_4_VECTOR, IPL2SOFT) nrf_irq_handler(void) {
    NRF_Status sts = NRF_GetStatus();
    DEBUG("%s() Status: 0x%02X", __func__, sts.status);
    
    if(sts.rx_data_ready) {
        
        NRF_CleanInterrupts(RX_DR);
    }
    if(sts.tx_data_sent) {
        
        NRF_CleanInterrupts(TX_DS);
    }
    if(sts.max_retransmissions) {
        
        NRF_CleanInterrupts(MAX_RT);
    }
    
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_EXTERNAL_4);
}

void NRFAPP_Initialize(void) {
    /* Place the App state machine in its initial state. */
    nrfappData.state = NRFAPP_STATE_INIT;
    
    // Address
    nrfappData.aw_bytes = AW_TO_BYTES(AW_5_BYTES);
    // Broadcast address, common to all devices
    nrfappData.pipe0 = 0xDEADBEEF00;
    nrfappData.pipe1 = 0xAABBCCDD01;
    // TODO get the serial number (4bytes)
    // TODO pipe 1-5 will be numbered <serial>x where x 1-5
//    const uint8_t pipeBaseaddress[] = { 0x01, 0xCE, 0xCA, 0xEF, 0xBE };
//    memcpy(nrfappData.pipe0, pipe0address, nrfappData.aw_bytes);
    
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

void configure_irq() {
    DEBUG("%s()", __func__);
    PLIB_INT_Enable(INT_ID_0);
    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_EXTERNAL_4);
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_EXTERNAL_4);
    PLIB_INT_ExternalFallingEdgeSelect(INT_ID_0, INT_SOURCE_EXTERNAL_4);
}

void NRFAPP_Tasks(void) {
    /* Check the application's current state. */
    switch (nrfappData.state) {
            /* Application's initial state. */
        case NRFAPP_STATE_INIT:
        {
            if (configureSPI()) {
                INFO("NRF App started!");
                configure_irq();
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
            NRF_Status sts;
            NRF_Initialize();
            NRF_SetAddressWidth(BYTES_TO_AW(nrfappData.aw_bytes));
#if SOM_MASTER
            NRF_OpenReadingPipe(1, nrfappData.pipe0);
            NRF_OpenWritingPipe(nrfappData.pipe1);
#else
            NRF_OpenReadingPipe(0, nrfappData.pipe1);
            NRF_OpenWritingPipe(nrfappData.pipe0);
#endif
            nrfappData.state = NRFAPP_STATE_IDLE;
            break;
        }

        case NRFAPP_STATE_IDLE: {

            break;
        }

        case NRFAPP_STATE_PWR_TX: {
                                      NRF_SetMode(0); // Tx
                                      NRF_PowerEnable(true);
                                      // WIP

                                      break;
        }

        default:
        {
            break;
        }
    }
}
