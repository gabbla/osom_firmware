#include "nrfapp.h"

NRFAPP_DATA nrfappData;

void printNRFStatus(const NRF_Status *status);

void __ISR(_EXTERNAL_4_VECTOR, IPL1AUTO) nrf_irq_handler(void) {
    NRF_Status sts = NRF_GetStatus();
    printNRFStatus(&sts);

    if (sts.rx_data_ready) {
        NRF_CleanInterrupts(RX_DR);
    }
    if (sts.tx_data_sent) {
        NRF_CleanInterrupts(TX_DS);
    }
    if (sts.max_retransmissions) {
        NRF_FlushTx();
        NRF_CleanInterrupts(MAX_RT);
    }

    NRF_CleanInterrupts(RX_DR | TX_DS | MAX_RT);
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_EXTERNAL_4);
}

void NRFAPP_Initialize(void) {
    /* Place the App state machine in its initial state. */
    nrfappData.state = NRFAPP_STATE_INIT;

    // Address
    nrfappData.aw_bytes = AW_TO_BYTES(AW_5_BYTES);
    // Broadcast address, common to all devices
    nrfappData.pipe0 = 0x314E6F6465; // 1Node
    nrfappData.pipe1 = 0x324e6f6465; // 2Node
    nrfappData.pipe0 = 0x65646F4E31; // 1Node
    nrfappData.pipe1 = 0x65646F4E32; // 2Node
    // TODO get the serial number (4bytes)
    // TODO pipe 1-5 will be numbered <serial>x where x 1-5
    //    const uint8_t pipeBaseaddress[] = { 0x01, 0xCE, 0xCA, 0xEF, 0xBE };
    //    memcpy(nrfappData.pipe0, pipe0address, nrfappData.aw_bytes);

    nrfappData.device_type = 1; // 1 = RX | 0 = TX
    INFO("Bootloader PIN: %d", nrfappData.device_type);
}

int8_t initializeNRFappMailbox() {
    nrfappData.commandMailBox = SYS_MSG_MailboxOpen(NRF_MAILBOX, NULL);
    if (nrfappData.commandMailBox == SYS_OBJ_HANDLE_INVALID) {
        ERROR("Failed to open Command Mail Box");
        return -1;
    } else {
        DEBUG("NRFApp mailbox is open");
    }

    SYS_OBJ_HANDLE msgType =
        SYS_MSG_TypeCreate(NRF_MAILBOX, NRF_MSG_ID, NRF_MSG_PRIORITY);
    // Add the message type
    if (msgType != SYS_OBJ_HANDLE_INVALID) {
        SYS_MSG_MailboxMsgAdd(nrfappData.commandMailBox, msgType);
        DEBUG("Subuscribed to NRFApp mailbox");
        return 0;
    }
    WARN("Subscription to NRFApp mailbox failed!");
    return -2;
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
    PLIB_SPI_FIFODisable(SPI_ID_1);
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
    PLIB_INT_VectorPrioritySet(INT_ID_0, INT_VECTOR_INT4, INT_PRIORITY_LEVEL1);
    PLIB_INT_VectorSubPrioritySet(INT_ID_0, INT_VECTOR_INT4, INT_SUBPRIORITY_LEVEL0);
    //PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_EXTERNAL_4);
    PLIB_INT_ExternalFallingEdgeSelect(INT_ID_0, INT_SOURCE_EXTERNAL_4);
    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_EXTERNAL_4);
}

void printOnceState(const NRFAPP_STATES state, const char *strstatus) {
    static NRFAPP_STATES s = 123;
    if (state != s) {
        s = state;
        DEBUG("New state: %s", strstatus);
    }
}

void printNRFStatus(const NRF_Status *status) {
    DEBUG("STATUS:      0x%02X", status->status);
    DEBUG(" TX_FULL:    %d", status->tx_full);
    DEBUG(" RX_PIPE_NUM 0x%02X", status->rx_pipe_number);
    DEBUG(" MAX_RET     %d", status->max_retransmissions);
    DEBUG(" TX_DS       %d", status->tx_data_sent);
    DEBUG(" RX_DR       %d", status->rx_data_ready);
}

void printNRFInfo(const NRF_Info *info) {
    printNRFStatus(&info->status);
    DEBUG("TX_ADDR:     0x%02X", info->tx_addr);
    DEBUG("EN_AA:       0x%02X", info->en_aa);
    DEBUG("EN_RXADDR:   0x%02X", info->en_rxaddr);
    DEBUG("RF_CH:       0x%02X", info->rf_ch);
    DEBUG("RF_SETUP:    0x%02X", info->rf_setup);
    DEBUG("CONFIG:      0x%02X", info->config);
    DEBUG("DYNPD:       0x%02X", info->dynpd);
}

void NRFAPP_Tasks(void) {
    /* Check the application's current state. */
    switch (nrfappData.state) {
            /* Application's initial state. */
        case NRFAPP_STATE_INIT: {
            if (configureSPI()) {
                INFO("NRF App started!");
                DEBUG("Device type: %s [%d]", nrfappData.device_type ? "Master" : "Slave",
                      nrfappData.device_type);
                configure_irq();
                nrfappData.gpTimer = SYS_TMR_DelayMS(5000);
                nrfappData.state = NRFAPP_STATE_CONFIG;
                initializeNRFappMailbox();
            } else {
                ERROR("Failed to initialize NRF App!");
            }
            break;
        }

        case NRFAPP_STATE_CONFIG: {
            if (!SYS_TMR_DelayStatusGet(nrfappData.gpTimer)) break;
            printOnceState(NRFAPP_STATE_CONFIG, "config");
            nrfappData.gpTimer = SYS_TMR_HANDLE_INVALID;
            NRF_Status status = NRF_Initialize();
            if(!status.status) {
                WARN("Status is 0!");
            }
            DEBUG("Status is: 0x%02X", status);
            NRF_SetPALevel(NRF_PA_LOW);

            if(nrfappData.device_type) {
                NRF_OpenWritingPipe(nrfappData.pipe1);
                NRF_OpenReadingPipe(1, nrfappData.pipe0);
                nrfappData.state = NRFAPP_STATE_RX;
            } else {
                NRF_OpenWritingPipe(nrfappData.pipe0);
                NRF_OpenReadingPipe(1, nrfappData.pipe1);
                nrfappData.state = NRFAPP_STATE_TX;
                nrfappData.gpTimer = SYS_TMR_DelayMS(500);
            }
            NRF_StartListening();
            NRF_Info info = NRF_GetInfo();
            printNRFInfo(&info);
            break;
        }

        case NRFAPP_STATE_IDLE: {
            printOnceState(NRFAPP_STATE_IDLE, "IDLE");
            NRF_Status s = NRF_GetStatus();
            break;
        }

        case NRFAPP_STATE_PRE_TX: {
            printOnceState(NRFAPP_STATE_PRE_TX, "pre_tx");
            // TODO stop listening
            nrfappData.state = NRFAPP_STATE_TX;
            break;
        }

        case NRFAPP_STATE_TX: {
            if (!SYS_TMR_DelayStatusGet(nrfappData.gpTimer)) break;
            printOnceState(NRFAPP_STATE_TX, "tx");
            NRF_StopListening();
            static uint32_t cnt = 0xAABBCCDD;
            DEBUG("Writing 0x%X", cnt);
            NRF_Write(&cnt, sizeof(uint32_t));
            //nrfappData.state = NRFAPP_STATE_IDLE;
            ++cnt;
            NRF_StartListening();
            nrfappData.gpTimer = SYS_TMR_DelayMS(500);
            break;
        }

        case NRFAPP_STATE_PRE_RX: {
            printOnceState(NRFAPP_STATE_PRE_RX, "pre_rx");
            NRF_DumpRegisters();
            NRF_StartListening();
            nrfappData.state = NRFAPP_STATE_RX;
            break;
        }

        case NRFAPP_STATE_RX: {
            printOnceState(NRFAPP_STATE_RX, "rx");
            uint8_t pipe;
            if(NRF_Available(&pipe)) {
                INFO("Data available for pipe :%d", pipe);
                unsigned long got_time;
                NRF_ReadPayload(&got_time, sizeof(unsigned long));
                INFO("Got: %lu", got_time);
                //NRF_StopListening();
            }
            break;
        }

        default: {
            break;
        }
    }
}
