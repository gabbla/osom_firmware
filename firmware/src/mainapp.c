#include "mainapp.h"
MAINAPP_DATA mainappData;

/*
 * @brief Power on or off the given laser(s)
 * @param which Laser to be turned controlled, can be ORed (LASER_DX | LASER_SX)
 * @param power state to assign to the laser(s)
 */
void enableLaser(const uint8_t which, const bool power) {
    DEBUG("%s() which: 0x%02X power: %d", __func__, which, power);
    if (which & LASER_DX) {
        Channel_Enable(mainappData.channels[Channel_Right], power);
    } else {
        Channel_Enable(mainappData.channels[Channel_Right], false);
    }
    if (which & LASER_SX) {
        Channel_Enable(mainappData.channels[Channel_Left], power);
    } else {
        Channel_Enable(mainappData.channels[Channel_Left], false);
    }
}

void ChannelStatusCallback(uintptr_t context, uint32_t currTick) {
    MAINAPP_DATA *data = (MAINAPP_DATA *)context;
    DEBUG("%s() @ %lu ms", __func__, currTick);
    uint8_t channels = data->activeChannels;
    uint8_t ch = 0;
    uint8_t sts = 0;
    ChannelStatus temp_status;
    if (channels & LASER_DX) {
        temp_status = Channel_GetStatus(data->channels[Channel_Right]);
        ch |= LASER_DX;
        sts |= (temp_status == ChannelStatusActive ? LASER_DX : 0x00);
    }
    if (channels & LASER_SX) {
        temp_status = Channel_GetStatus(data->channels[Channel_Left]);
        ch |= LASER_SX;
        sts |= (temp_status == ChannelStatusActive ? LASER_SX : 0x00);
    }
    // This check fix an issue while entering the Positioning activity.
    // Since this callback will be called after SET_MODE (that turns on the
    // channels) the already turned on channels will be shut off
    if (data->runMode != RUN_MODE_POSITIONING) {
        enableLaser(LASER_DX | LASER_SX, false);
    }
    DEBUG("%s() Ch: 0x%02X Status: 0x%02X", __func__, ch, sts);

    Packet *reply = NULL;
    switch (data->runMode) {
        case RUN_MODE_LAP: {
            if (sts == ChannelStatusActive) break;
            reply = PACKET_CreateRunResult(ch, SYS_TMR_TickCountGet() * 1000);
            break;
        }
        default: {
            WARN("Dummy reply");
            reply = PACKET_Create();
            uint8_t dummy[] = {ch, sts};
            PACKET_SetDestination(reply, DEV_APPLICATION);
            PACKET_SetCommand(reply, BLE_CMD_CH_STATUS);
            PACKET_SetPayload(reply, dummy, 2);
            break;
        }
    }

    if (reply) SendPacketToBle(MSG_SRC_MAIN, reply);
}

void nextState(MAINAPP_STATES next) { mainappData.state = next; }

void MAINAPP_SetLedStatus(const LEDStatus status) {
    mainappData.ledStatus = status;
    LedStatusOff();
}

void positioningPhase() {
    // TODO manage the input states (I guess something getWatchdogDx status)
    DEBUG("Sending fake status");
    Packet *reply = PACKET_CreatePositionStatus(false, true);
    SendPacketToBle(MSG_SRC_MAIN, reply);
    nextState(MAINAPP_STATE_SERVICE_TASKS);
}

void BLE_CMD_MODE_Parser(const Packet *in, Packet *out, uintptr_t context) {
    DEBUG("%s()", __func__);
    MAINAPP_DATA *data = (MAINAPP_DATA *)context;

    if (PACKET_GetRunMode(in, &data->runMode, &data->activeChannels) > 0) {
        INFO("Entering new mode 0x%02X (channels 0x%02X)", data->runMode,
             data->activeChannels);
    }
    enableLaser(data->activeChannels, true);
    switch (data->runMode) {
        case RUN_MODE_FREE_START:
            break;
        case RUN_MODE_POSITIONING:
            break;
        case RUN_MODE_KO:
            break;
        case RUN_MODE_LAP:
            break;
        case RUN_MODE_NONE:
        default:
            enableLaser(LASER_SX | LASER_DX, false);
            break;
    }

    PACKET_SetCommand(out, BLE_CMD_RESPONSE);
    uint8_t res = CMD_RESPONSE_OK;
    PACKET_SetPayload(out, &res, 1);
}

void BLE_CMD_START_POS_Parser(const Packet *in, Packet *out,
                              uintptr_t context) {
    DEBUG("%s()", __func__);
    enableLaser(POS_GetLaser(in->payload), true);
    mainappData.phase = SP_POSITIONING_PHASE;
    // nextState(POSITIONING_PHASE);
}

void BLE_CMD_DONE_POS_Parser(const Packet *in, Packet *out, uintptr_t context) {
    DEBUG("%s()", __func__);
    enableLaser(LASER_DX | LASER_SX, false);
    nextState(MAINAPP_STATE_SERVICE_TASKS);
}

void BLE_CMD_GET_CHANNEL_STS_Parser(const Packet *in, Packet *out,
                                    uintptr_t context) {
    DEBUG("%s()", __func__);
    MAINAPP_DATA *data = (MAINAPP_DATA *)context;
    PACKET_SetCommand(out, BLE_CMD_RESPONSE);
    uint8_t res;
    uint8_t channel;
    if (PACKET_GetPayload(in, &channel) != 1) {
        ERROR("Cannot get the channel(s) to check");
        res = CMD_RESPONSE_FAIL;
    } else {
        data->activeChannels = channel;
        enableLaser(channel, true);
        if (SYS_TMR_CallbackSingle(100, context, ChannelStatusCallback) ==
            SYS_TMR_HANDLE_INVALID) {
            ERROR("%s() Failed to set timer callback", __func__);
            res = CMD_RESPONSE_FAIL;
        } else {
            res = CMD_RESPONSE_OK;
        }
    }
    PACKET_SetPayload(out, &res, 1);
}

// Single command parse function
static cmdParserFunction parsers[] = {
    &BLE_CMD_MODE_Parser,             // 0x10
    &BLE_CMD_START_POS_Parser,        // 0x11
    &BLE_CMD_DONE_POS_Parser,         // 0x12
    &BLE_CMD_GET_CHANNEL_STS_Parser,  // 0x13
};

void mcb(SYS_MSG_OBJECT *pMessage) {
    DEBUG("MAINAPP received from %d", pMessage->nSource);
}

int8_t initializeMainappMailbox() {
    mainappData.commandMailBox = SYS_MSG_MailboxOpen(MAIN_MAILBOX, NULL);
    if (mainappData.commandMailBox == SYS_OBJ_HANDLE_INVALID) {
        ERROR("Failed to open Command Mail Box");
        return -1;
    } else {
        DEBUG("Command Mail Box is open");
    }

    SYS_OBJ_HANDLE msgType =
        SYS_MSG_TypeCreate(MAIN_MAILBOX, MAIN_MSG_ID, MAIN_MSG_PRIORITY);
    // Add the message type
    if (msgType != SYS_OBJ_HANDLE_INVALID) {
        SYS_MSG_MailboxMsgAdd(mainappData.commandMailBox, msgType);
        DEBUG("Subuscribed to Main command");
        return 0;
    }
    WARN("Subscription to Main command failed!");
    return -2;
}

void MAINAPP_Initialize(void) {
    mainappData.state = MAINAPP_STATE_INIT;
    mainappData.phase = SP_IDLE;
    
    mainappData.configOffset = SOM_CFG_ADDRESS;
    OSAL_MUTEX_Create(&mainappData.configMutex);
}

uint32_t start = 0;

void dummyFinish(uintptr_t context, uint32_t currTick) {
    uint32_t stop = SYS_TMR_TickCountGet() - start;
    ChannelIndex idx = (ChannelIndex)context;
    INFO("%s() ch %d @ %d", __func__, idx, stop);
    Packet *p = PACKET_CreateRunResult(idx, stop);
    SendPacketToBle(MSG_SRC_MAIN, p);
}

SYS_TMR_HANDLE dummy;

#define RUN_TIMER_ID TMR_ID_3
static uint64_t cnt;
void __ISR(_TIMER_3_VECTOR, ipl1AUTO) runtimerHandler(void) {
    cnt++;
    StatusRightToggle();
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_TIMER_3);
    PLIB_TMR_Counter16BitSet(RUN_TIMER_ID, 65535);
}
void RUNTIMER_Init(TMR_MODULE_ID tmr) {
    PLIB_TMR_Stop(tmr);
    if (PLIB_TMR_ExistsMode32Bit(tmr)) PLIB_TMR_Mode16BitEnable(tmr);
    PLIB_TMR_Counter16BitClear(tmr);
    PLIB_TMR_ClockSourceSelect(tmr, TMR_CLOCK_SOURCE_PERIPHERAL_CLOCK);
    PLIB_TMR_PrescaleSelect(tmr, TMR_PRESCALE_VALUE_1);
    PLIB_TMR_Counter16BitSet(tmr, 65535);

    // Interrupt
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_TIMER_3);
    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_TIMER_3);
    PLIB_INT_VectorPrioritySet(INT_ID_0, INT_VECTOR_T3, INT_PRIORITY_LEVEL1);
}

void RUNTIMER_Start(TMR_MODULE_ID tmr) {
    cnt = 0;
    PLIB_TMR_Start(tmr);
}

uint64_t RUNTIMER_Stop(TMR_MODULE_ID tmr) {
    PLIB_TMR_Stop(tmr);
    return cnt;
}

void wdcb(const ChannelIndex idx, const ChannelStatus s, uintptr_t *cntx) {
    INFO("Channel %d new state is %d @ %d", idx, s, SYS_TMR_TickCountGetLong());
    MAINAPP_DATA *data = (MAINAPP_DATA *)cntx;

    Packet *reply = NULL;
    switch (data->runMode) {
        case RUN_MODE_POSITIONING: {
            Packet *reply = PACKET_CreatePositionStatus(idx, s);
            PACKET_SetDestination(reply, DEV_APPLICATION);
            SendPacketToBle(MSG_SRC_MAIN, reply);
            break;
        }
        case RUN_MODE_FREE_START: {
            if (s == ChannelStatusInactive) {
                start = SYS_TMR_TickCountGet();
                Packet *reply = PACKET_CreateGateCrossPacket(idx);
                PACKET_SetDestination(reply, DEV_APPLICATION);
                SendPacketToBle(MSG_SRC_MAIN, reply);
                Channel_Enable(Channel_Get(idx), false);
                dummy = SYS_TMR_CallbackSingle(5000, NULL, dummyFinish);
            }
            break;
        }
        case RUN_MODE_LAP: {
            if (s == ChannelStatusActive) break;
            reply = PACKET_CreateRunResult(idx, SYS_TMR_TickCountGet());
            break;
        }
        default: {
            WARN("Dummy reply");
            reply = PACKET_Create();
            uint8_t dummy[] = {idx, s};
            PACKET_SetDestination(reply, DEV_APPLICATION);
            PACKET_SetCommand(reply, BLE_CMD_CH_STATUS);
            PACKET_SetPayload(reply, dummy, 2);
            break;
        }
    }

    if (reply) SendPacketToBle(MSG_SRC_MAIN, reply);
}

void BatteryCallback(BQ27441_Command cmd, uint8_t *data, size_t s,
                     uintptr_t user_data) {
    if (!data) {
        ERROR("Invalid data received! [0x%02X]", cmd);
        return;
    }
    switch (cmd) {
        case BQ27441_STATE_OF_CHARGE: {
            soc_t soc = BQ27441_GetStateOfCharge(data);
            bool charging = !BQ_PGoodStateGet();
            DEBUG("State of charge: %d %%", soc);
            Packet *p = PACKET_CreateBatteryPacket(charging, soc);
            SendPacketToBle(MSG_SRC_MAIN, p);
            break;
        }
        case BQ27441_VOLTAGE: {
            DEBUG("Voltage: %d mV", BQ27441_GetMillivolts(data));
            break;
        }
        case BQ27441_AVERAGE_CURRENT: {
            DEBUG("Average current: %d mAh", BQ27441_GetAverageCurrent(data));
            break;
        }
    }
}

void batteryInfoCallback(uintptr_t context, uint32_t currTick) {
    DEBUG("[%llu] Asking battery info..", currTick);
    BQ27441_GetData(BQ27441_STATE_OF_CHARGE, &BatteryCallback, NULL);
    BQ27441_GetData(BQ27441_VOLTAGE, &BatteryCallback, NULL);
    BQ27441_GetData(BQ27441_AVERAGE_CURRENT, &BatteryCallback, NULL);
}

void CFG_Print(const SOMConfig *cfg) {
    if(!cfg) {
        ERROR("%s() Config is NULL", __func__);
        return;
    }
    INFO("***** CONFIG  *****");
    INFO("Offset: 0x%04X", cfg->offset);
    INFO("Version: 0x%02X", cfg->version);
    INFO("Name: '%s'", cfg->name);
    INFO("Role: %s", cfg->flags.role? "master" : "slave");
    if(cfg->flags.role == SOM_CFG_ROLE_SLAVE) {
        INFO("Slave ID: 0x%02X", cfg->flags.slave_id);
    }
    INFO("*****   END   *****");
}

bool MAINAPP_IsConfigLoaded() {
    bool ret = false;
    if(OSAL_MUTEX_Lock(&mainappData.configMutex, 1000) == OSAL_RESULT_TRUE) {
        ret = CFG_IsValid(&mainappData.config);
        OSAL_MUTEX_Unlock(&mainappData.configMutex);
    }
    return ret;
}

const SOMConfig *MAINAPP_GetConfig() {
    SOMConfig *cfg = NULL;
    if(OSAL_MUTEX_Lock(&mainappData.configMutex, 1000) == OSAL_RESULT_TRUE) {
        cfg = &mainappData.config;
        OSAL_MUTEX_Unlock(&mainappData.configMutex);
    }
    return cfg;
}

void MAINAPP_Tasks(void) {
    /* Check the application's current state. */
    switch (mainappData.state) {
        /* Application's initial state. */
        case MAINAPP_STATE_INIT: {
            bool appInitialized = true;
            initializeMainappMailbox();
            size_t ch;
            for (ch = 0; ch < Channel_Max; ++ch) {
                mainappData.channels[ch] = Channel_Get((ChannelIndex)ch);
                Channel_SetCallback(mainappData.channels[ch], wdcb,
                                    (uintptr_t *)&mainappData);
            }

            //            mainappData.batteryInfoTmr =
            //            SYS_TMR_CallbackPeriodic(10000,
            //             NULL, batteryInfoCallback);

            // I2C stuff
            DEBUG("I2C driver version: %s", DRV_I2C_VersionStrGet(0));
            mainappData.hEeprom = DRV_I2C_Open(DRV_I2C_INDEX_0, 
                    DRV_IO_INTENT_READWRITE | DRV_IO_INTENT_SHARED);
            if (mainappData.hEeprom == DRV_HANDLE_INVALID) {
                ERROR("EEPROM handler is invalid");
                appInitialized = false;
            } else {
                DEBUG("EEPROM handler is valid");
            }
            
            if (appInitialized) {
                INFO("Main App started!");
                mainappData.state = MAINAPP_STATE_LOAD_CFG;
                //RUNTIMER_Init(RUN_TIMER_ID);
                //RUNTIMER_Start(RUN_TIMER_ID);
            }
            break;
        }

        case MAINAPP_STATE_LOAD_CFG: {
            DEBUG("Loading config");
            if(OSAL_MUTEX_Lock(&mainappData.configMutex, 1000) == OSAL_RESULT_TRUE) {
                mainappData.eepromBuffHandler = DRV_I2C_TransmitThenReceive(
                        mainappData.hEeprom, 0xA0, &mainappData.configOffset, 2,
                        &mainappData.config, sizeof(SOMConfig), NULL);
                if(mainappData.eepromBuffHandler == DRV_I2C_BUFFER_HANDLE_INVALID) {
                    ERROR("Error while loading config");
                    mainappData.state = MAINAPP_STATE_IDLE;
                    OSAL_MUTEX_Unlock(&mainappData.configMutex);
                    // TODO handle this kind of error
                    break;
                }
                mainappData.state = MAINAPP_STATE_WAIT_CFG;
            } else {
                ERROR("Cannot lock configMutex");
            }
            break;
        }
        
        case MAINAPP_STATE_WRITE_CFG: {
            DEBUG("Writing config");
            CFG_Print(&mainappData.config);
            mainappData.eepromBuffHandler = DRV_I2C_Transmit(
                    mainappData.hEeprom, 0xA0,
                    &mainappData.config, sizeof(SOMConfig), NULL);
            if(mainappData.eepromBuffHandler == DRV_I2C_BUFFER_HANDLE_INVALID) {
                ERROR("Error while writing config");
                mainappData.state = MAINAPP_STATE_IDLE;
                // TODO handle this kind of error
                break;
            }
            mainappData.state = MAINAPP_STATE_WAIT_CFG;
            break;
        }
        
        case MAINAPP_STATE_WAIT_CFG: {
            switch(DRV_I2C_TransferStatusGet(mainappData.hEeprom,
                    mainappData.eepromBuffHandler)) {
                case DRV_I2C_BUFFER_EVENT_COMPLETE:
                    DEBUG("Config loaded");
                    // TODO is valid?
                    if(!CFG_IsValid(&mainappData.config)) {
                        WARN("Config is not valid");
                        CFG_SetDefault(&mainappData.config);
                        mainappData.state = MAINAPP_STATE_WRITE_CFG;
                        break;
                    }
                    CFG_Print(&mainappData.config);
                    mainappData.state = MAINAPP_STATE_SERVICE_TASKS;
                    // Unlock the mutex
                    OSAL_MUTEX_Unlock(&mainappData.configMutex);
                    break;
                case DRV_I2C_BUFFER_EVENT_ERROR:
                    ERROR("Error while reading config");
                    mainappData.state = MAINAPP_STATE_IDLE;
                    // Unlock the mutex
                    OSAL_MUTEX_Unlock(&mainappData.configMutex);
                    // TODO handle this error
                    break;
            }            
            break;
        }
       
        case MAINAPP_STATE_SERVICE_TASKS: {
            LED_Tasks();
            MSG_Tasks();
            MODE_Tasks();
            break;
        }

        case MAINAPP_STATE_IDLE: {
            break;
        }
        
        default: {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

void LED_Tasks() {
    // TODO reflect system status
    // - Slave conneted/disconnected
    static uint32_t cnt = 0;
    switch (mainappData.ledStatus) {
        case LED_STATUS_INVALID:
        case LED_STATUS_DISCOVERED:
        case LED_STATUS_DISCONNECTED: {
            // TODO delay
            if (SYS_TMR_TickCountGet() - cnt >= 500) {
                LedStatusToggle();
                cnt = SYS_TMR_TickCountGet();
            }
            break;
        }
        case LED_STATUS_CONNECTED: {
            static uint8_t loop = 0;
            if (loop < 4) {
                if (SYS_TMR_TickCountGet() - cnt >= 150) {
                    LedStatusToggle();
                    cnt = SYS_TMR_TickCountGet();
                    loop++;
                }
            } else {
                if (SYS_TMR_TickCountGet() - cnt >= 1000) {
                    loop = 0;
                    cnt = SYS_TMR_TickCountGet();
                }
            }
            break;
        }
        default:
            LedStatusOff();
            break;
    }
}

void MSG_Tasks() {
    // Check for messages
    SYS_MSG_OBJECT *next;
    if ((next = SYS_MSG_MailboxMessagesGet(mainappData.commandMailBox)) !=
        NULL) {
        DEBUG("Found a message from %d", next->nSource);
        Packet *p = (Packet *)next->pData;
        size_t parserIndex = p->cmd - MAINAPP_CMD_OFFSET;
        if (p->cmd >= BLE_CMD_MAX_CMD || p->cmd < MAINAPP_CMD_OFFSET ||
            (parsers[parserIndex] == NULL)) {
            ERROR("Command 0x%02X not supported!", p->cmd);
            // TODO send "Not supported"
            Packet *notSupported = PACKET_Create();
            notSupported->cmd = BLE_CMD_NOT_SUPPORTED;
            // notSupported->tid = PACKET_GetTransactionID(p);
            SendPacketToBle(MSG_SRC_MAIN, notSupported);
        } else {
            DEBUG("Parsing command ID 0x%02X", p->cmd);
            // TODO manage the destination
            if(p->dst & DEV_ALL_SLAVES) {
                DEBUG("===== Notify slaves too");
                // Forward to slaves
                SYS_MSG_OBJECT message;
                message.nMessageTypeID = NRF_MSG_ID;
                message.nSource = MSG_SRC_MAIN;
                message.nSizeData = sizeof(Packet);
                message.pData = (uintptr_t *)p;
                SYS_MSG_RESULTS myRes =
                    SYS_MSG_MessageSend(MAIN_MAILBOX, &message);
                if (myRes != SYS_MSG_SENT) ERROR("Failed to send!! %d", myRes);
            }
            // TODO reply may not be generated by every command.
            // It's better to generate it inside the parser, if needed
            Packet *reply = PACKET_CreateForReply(p);
            parsers[parserIndex](p, reply, (uintptr_t)&mainappData);
            SendPacketToBle(MSG_SRC_MAIN, reply);
        }
        PACKET_Free(p);  // really important
    }
}

void MODE_Tasks() {
    switch (mainappData.runMode) {
        case RUN_MODE_FREE_START: {
            break;
        }
    }
}
