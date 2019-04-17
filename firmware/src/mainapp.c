#include "mainapp.h"
MAINAPP_DATA mainappData;


/*
 * @brief Power on or off the given laser(s)
 * @param which Laser to be turned controlled, can be ORed (LASER_DX | LASER_SX)
 * @param power state to assign to the laser(s)
 */
void enableLaser(const uint8_t which, const bool power){
    size_t i;
    Laser *laser;
    // FIXME does not work well with 2 channels
    for(i = 0; i < 2; ++i){
        if((1 << i) & which)
            Channel_Enable(mainappData.channels[i], power);
    }
}

void nextState(MAINAPP_STATES next){
    mainappData.state = next;
}

void positioningPhase(){
    // TODO manage the input states (I guess something getWatchdogDx status)
    DEBUG("Sending fake status");
    Packet *reply = PACKET_CreatePositionStatus(false, true);
    SendPacketToBle(MSG_SRC_MAIN, reply);
    nextState(MAINAPP_STATE_SERVICE_TASKS);
}

void BLE_CMD_MODE_Parser(const Packet *in, Packet *out, uintptr_t context){
    DEBUG("%s()", __func__);
    MAINAPP_DATA *data = (MAINAPP_DATA*)context;

    if(PACKET_GetRunMode(in, &data->runMode, &data->activeChannels) > 0) {
        INFO("Entering new mode 0x%02X (channels 0x%02X)",
                data->runMode, data->activeChannels);
    }
}

void BLE_CMD_START_POS_Parser(const Packet *in, Packet *out, uintptr_t context){
    DEBUG("%s()", __func__);
    enableLaser(POS_GetLaser(in->payload), true);
    mainappData.phase = SP_POSITIONING_PHASE;
    //nextState(POSITIONING_PHASE);
}

void BLE_CMD_DONE_POS_Parser(const Packet *in, Packet *out, uintptr_t context){
    DEBUG("%s()", __func__);
    enableLaser(LASER_DX | LASER_SX, false);
    nextState(MAINAPP_STATE_SERVICE_TASKS);
}

// Single command parse function
static cmdParserFunction parsers[] = {
    &BLE_CMD_MODE_Parser,           // 0x10
    &BLE_CMD_START_POS_Parser,      // 0x11
    &BLE_CMD_DONE_POS_Parser,       // 0x12
};

int8_t initializeMainappMailbox(){
    mainappData.commandMailBox = SYS_MSG_MailboxOpen(MAIN_MAILBOX, NULL);
    if(mainappData.commandMailBox == SYS_OBJ_HANDLE_INVALID) {
        ERROR("Failed to open Command Mail Box");
        return -1;
    } else {
        DEBUG("Command Mail Box is open");
    }

    SYS_OBJ_HANDLE msgType = SYS_MSG_TypeCreate(MAIN_MAILBOX, MAIN_MSG_ID, MAIN_MSG_PRIORITY);
    // Add the message type
    if(msgType != SYS_OBJ_HANDLE_INVALID){
        SYS_MSG_MailboxMsgAdd(mainappData.commandMailBox, msgType);
        DEBUG("Subuscribed to Main command");
        return 0;
    }
    WARN("Subscription to Main command failed!");
    return -2;
}

void MAINAPP_Initialize ( void )
{
    mainappData.state = MAINAPP_STATE_INIT;
    mainappData.phase = SP_IDLE;
}

void wdcb(const ChannelIndex idx, const ChannelStatus s, uintptr_t *cntx){
    INFO("Channel %d new state is %d", idx, s);
    MAINAPP_DATA *data = (MAINAPP_DATA*)cntx;
    switch(data->phase) {
        case SP_POSITIONING_PHASE: {
            Packet *reply = PACKET_CreatePositionStatus(idx, s);
            SendPacketToBle(MSG_SRC_MAIN, reply);
            break;
        }
    }
}

void BatteryCallback(BQ27441_Command cmd, uint8_t *data, size_t s, uintptr_t user_data) {
    if(!data) {
        ERROR("Invalid data received! [0x%02X]", cmd);
        return;
    }
    switch(cmd) {
        case BQ27441_STATE_OF_CHARGE: {
            DEBUG("State of charge: %d %%", BQ27441_GetStateOfCharge(data));
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

void MAINAPP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( mainappData.state )
    {
        /* Application's initial state. */
        case MAINAPP_STATE_INIT:
        {
            bool appInitialized = true;
            initializeMainappMailbox();
            size_t ch;
            for(ch = 0; ch < Channel_Max; ++ch){
                mainappData.channels[ch] = Channel_Get((ChannelIndex)ch);
                Channel_SetCallback(mainappData.channels[ch], wdcb, (uintptr_t *)&mainappData);
            }
            
            //mainappData.batteryInfoTmr = SYS_TMR_CallbackPeriodic(60000, NULL, batteryInfoCallback);
            
            if (appInitialized)
            {
                INFO("Main App started!");
                mainappData.state = MAINAPP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case MAINAPP_STATE_SERVICE_TASKS:
        {
            LED_Tasks();
            MSG_Tasks();
            break;
        }

        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

void LED_Tasks() {
    // TODO reflect system status
    // - Slave conneted/disconnected
}

void MSG_Tasks() {
    // Check for messages
    SYS_MSG_OBJECT *next;
    if((next = SYS_MSG_MailboxMessagesGet(mainappData.commandMailBox)) 
            != NULL){
        DEBUG("Found a message from %d", next->nSource);
        Packet *p = (Packet*)next->pData;
        size_t parserIndex = p->cmd - MAINAPP_CMD_OFFSET;
        if(p->cmd >= BLE_CMD_MAX_CMD 
                || p->cmd < MAINAPP_CMD_OFFSET
                || (parsers[parserIndex] == NULL)) {
            ERROR("Command 0x%02X not supported!", p->cmd);
            // TODO send "Not supported"
            Packet *notSupported = PACKET_Create();
            notSupported->cmd = BLE_CMD_NOT_SUPPORTED;
            notSupported->tid = PACKET_GetTransactionId(p);
            SendPacketToBle(MSG_SRC_MAIN, notSupported);
        } else {
            DEBUG("Parsing command ID 0x%02X", p->cmd);
            Packet *reply = PACKET_CreateForReply(p);
            parsers[parserIndex](p, reply, (uintptr_t)&mainappData);
            SendPacketToBle(MSG_SRC_MAIN, reply);
        }
        PACKET_Free(p); // really important
    }
}
