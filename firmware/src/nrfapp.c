#include "nrfapp.h"

#define IS_MASTER(x) ( x.device_type == DEVICE_TYPE_MASTER )

#define MAX_DISCOVERY   10

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
    nrfappData.pipe0 = 0x65646F4E31; // 1Node
    nrfappData.pipe1 = 0x65646F4E32; // 2Node
    // TODO get the serial number (4bytes)
    // TODO pipe 1-5 will be numbered <serial>x where x 1-5
    //    const uint8_t pipeBaseaddress[] = { 0x01, 0xCE, 0xCA, 0xEF, 0xBE };
    //    memcpy(nrfappData.pipe0, pipe0address, nrfappData.aw_bytes);

    nrfappData.device_type = 0; // 1 = RX | 0 = TX
    INFO("Bootloader PIN: %d", nrfappData.device_type);
    
    // Clear device discovered
    memset(nrfappData.deviceDiscovered, 0, sizeof(nrfappData.deviceDiscovered));
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
    // Selecting Reference clock (MCLK) as source clock for this SPI
    // MCLK value is stored in SYS_CLK_BUS_REFERENCE_1 and now is 18 Mhz
    // This output around 9 Mhz clock
    PLIB_SPI_BaudRateClockSelect(SPI_ID_1, SPI_BAUD_RATE_MCLK_CLOCK);
    PLIB_SPI_BaudRateSet(SPI_ID_1, SYS_CLK_BUS_REFERENCE_1, 1000000);
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
    NRF_CSOn();
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

void addDiscoveredDevice(const AnnouncePayload *ap) {
    size_t i;
    SOM_Device *dd = nrfappData.deviceDiscovered;
    for(i = 0; i < 5; ++i) {
        if(!dd[i].valid) {
            memcpy(dd[i].sn, ap->sn, sizeof(ap->sn));
            dd[i].valid = true;
            break;
        }
    }
}

uint8_t getDeviceDiscoveredCnt() {
    size_t i = 0;
    SOM_Device *dd = nrfappData.deviceDiscovered;
    for(; i < 5; ++i) {
        if(!nrfappData.deviceDiscovered[i].valid) {
            break;
        }
    }
    return i;
}

bool isChargerConnected() {
    return !BQ_PGoodStateGet();
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
                //configure_irq();
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

            if(!IS_MASTER(nrfappData)) {
                NRF_OpenWritingPipe(nrfappData.pipe1);
                NRF_OpenReadingPipe(1, nrfappData.pipe0);
                nrfappData.state = NRF_STATE_DISCOVERY;
            } else {
                NRF_OpenWritingPipe(nrfappData.pipe0);
                NRF_OpenReadingPipe(1, nrfappData.pipe1);
                nrfappData.state = NRF_STATE_ANNOUNCE;
            }

            NRF_StartListening();
            break;
        }

        case NRF_STATE_DISCOVERY: {
            printOnceState(NRF_STATE_DISCOVERY, "discovery");
            if(nrfappData.discoveryCnt >= MAX_DISCOVERY) {
                WARN("Discovery ended");
                DEBUG("Device found:");
                size_t i;
                for(i = 0; i < 5; ++i)
                    if(nrfappData.deviceDiscovered[i].valid)
                        DEBUG("\t %d) %s", i, nrfappData.deviceDiscovered[i].sn);
                nrfappData.state = NRFAPP_STATE_IDLE;
                break;
            }
            NRF_StopListening();
            // Ask if anyone is there
            Packet *p = PACKET_CreateDiscovery();
            NRF_Write(p, PACKET_GetFullSize(p));
            PACKET_Free(p);
            nrfappData.discoveryTimeout = SYS_TMR_TickCountGet();
            DEBUG("Sending discovery @ %d [%d/%d]", nrfappData.discoveryTimeout,
                  ++nrfappData.discoveryCnt, MAX_DISCOVERY);
            NRF_StartListening();
            nrfappData.state = NRF_STATE_WAIT_DISCOVERY;
            break;
        }

        case NRF_STATE_WAIT_DISCOVERY: {
            if (SYS_TMR_TickCountGet() - nrfappData.discoveryTimeout > 1000) {
                nrfappData.state = NRF_STATE_DISCOVERY;
                break;
            }
            if(NRF_Available(NULL)) {
                // Someone may have replied us
                uint8_t data[32];
                NRF_ReadPayload(data, 32);
                Packet *p = PACKET_Get(data); 
                AnnouncePayload ap = PACKET_GetAnnouncePayload(p);
                DEBUG("SN: %s", ap.sn);
                DEBUG("SOC: %d %", ap.soc);
                DEBUG("Charging: %s", ap.charging? "yes" : "no");
                addDiscoveredDevice(&ap);
                DEBUG("Device discovered: %d", getDeviceDiscoveredCnt());
                // Send over BLE
                Packet *ble = PACKET_CreateNewSlave(&ap); // Freed by BLEApp
                SendPacketToBle(MSG_SRC_NRF, ble);
                // ACK the discovery
                // TODO assign a role
                Packet *reply = PACKET_CreateForReply(p);
                PACKET_SetCommand(reply, BLE_CMD_DISCOVERY_ACK);
                NRF_StopListening();
                NRF_Write(reply, PACKET_GetFullSize(reply));
                NRF_StartListening();
                PACKET_Free(p);
                PACKET_Free(reply);
            }
            break;
        }

        case NRF_STATE_ANNOUNCE: {
            printOnceState(NRF_STATE_ANNOUNCE, "announce");
            if(!nrfappData.discovered && NRF_Available(NULL)) {
                uint8_t data[32];
                NRF_ReadPayload(data, 32);
                Packet *p = PACKET_Get(data);
                if(PACKET_GetCommand(p) == BLE_CMD_DISCOVERY) {
                    DEBUG("Got discovery");
                    uint8_t sn[5] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE};
                    Packet *r = PACKET_CreateAnnounce(sn, 88, isChargerConnected());
                    NRF_StopListening();
                    NRF_WritePacket(r);
                    NRF_StartListening();
                    // wait ack
                    PACKET_Free(r);
                    nrfappData.state = NRF_STATE_WAIT_DISCOVERY_ACK;
                    //nrfappData.state = NRFAPP_STATE_IDLE;
                    nrfappData.discoveryAckTimeout = SYS_TMR_TickCountGet();
                }
                PACKET_Free(p);
            }
            break;
        }

        case NRF_STATE_WAIT_DISCOVERY_ACK: {
            printOnceState(NRF_STATE_WAIT_DISCOVERY_ACK, "discovery ack");
            if(SYS_TMR_TickCountGet() - nrfappData.discoveryAckTimeout > 1000) {
                WARN("Discovery not ACKed!");
                nrfappData.state = NRF_STATE_ANNOUNCE;
                break;
            }
            if(NRF_Available(NULL)) {
                uint8_t data[32];
                NRF_ReadPayload(data, 32);
                Packet *p = PACKET_Get(data);
                if(PACKET_GetCommand(p) == BLE_CMD_DISCOVERY_ACK) {
                    DEBUG("Discovery ACKed");
                    nrfappData.discovered = true;
                    nrfappData.state = NRFAPP_STATE_IDLE;
                    // TODO notify mainapp for led status
                }
                PACKET_Free(p);
            }
            break;
        }
        
        case NRFAPP_STATE_IDLE: {
            printOnceState(NRFAPP_STATE_IDLE, "IDLE");
            //NRF_Status s = NRF_GetStatus();
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
                uint64_t got_time;
                NRF_ReadPayload(&got_time, sizeof(uint64_t));
                INFO("Got: %lu", got_time);              
                NRF_StopListening();
                NRF_Write(&got_time, sizeof(uint64_t));
                DEBUG("Sent response");
                NRF_StartListening();
                
            }
            break;
        }

        default: {
            break;
        }
    }
}
