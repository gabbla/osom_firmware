#include "mainapp.h"
MAINAPP_DATA mainappData;

#if 0
void kickFakeWatchdog3(){
    TMR4 = 0;
}

void __ISR(_EXTERNAL_4_VECTOR, single) leftInt(){
    //kickFakeWatchdog3();
    FakeWD_Kick(mainappData.rightWD);
    IFS0bits.INT4IF = 0; // clear the flag
}

void setupLaserCapture() {
    // Set up left channel (INT4)
    INTCONbits.INT4EP = 0; // Falling edge
    IFS0bits.INT4IF = 0; // clear the flag
    IEC0bits.INT4IE = 1; // enable the interrupt
    IPC4bits.INT4IP = 7; // max priority
}

void __ISR(_TIMER_4_VECTOR, single) watchdog3() {
    INFO("!!!!! OBSTACLE !!!!!");
    IFS0bits.T4IF = 0; // Clear the flag
}

void enableFakeWatchdog3(const bool en){
    T4CONbits.ON = en;
}

void setupFakeWatchdog3() {
    // Faking a watchdog with timer 4
    // Kick window 550 us
    T4CONbits.ON = 0; // Turn off the timer
    T4CONbits.TCS = 0; // Internal clock source
    T4CONbits.T32 = 0; // TMRx and TMRy 2 16bits timer
    T4CONbits.TCKPS = 3; // Prescaler to 1:8
    PR4 = 2750; // Period set to 0.55 ms

    // Enable interrupt
    IFS0bits.T4IF = 0; // Clear the flag
    IEC0bits.T4IE = 1; // Enable the interrupt
    IPC4bits.T4IP = 7; //High priority
}
#endif

/*
 * @brief Power on or off the given laser(s)
 * @param which Laser to be turned controlled, can be ORed (LASER_DX | LASER_SX)
 * @param power state to assign to the laser(s)
 */
void enableLaser(const uint8_t which, const bool power){
    size_t i;
    Laser *laser;
    for(i = 0; i < 2; ++i){
        // TODO move to channel
        //laser = &lasers[i];
        //if(i & which) 
        //    PLIB_PORTS_PinWrite(PORTS_ID_0, laser->port, laser->pin, power);
        
        if((1 << i) & which)
            Channel_Enable(mainappData.channels[i], power);
        
    }
    // start or stop the modulation
    //enableLaserModulation(power);
    //LaserModulatorIfc_Enable(mainappData.modulator, power);
    //enableFakeWatchdog3(power);
    //FakeWD_Enable(mainappData.rightWD, power);
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

void BLE_CMD_MODE_Parser(const Packet *in, Packet *out){
    DEBUG("%s()", __func__);
    enableLaser(MODE_GetLaser(in->payload), true);

}

void BLE_CMD_START_POS_Parser(const Packet *in, Packet *out){
    DEBUG("%s()", __func__);
    enableLaser(POS_GetLaser(in->payload), true);
    nextState(POSITIONING_PHASE);
}

void BLE_CMD_DONE_POS_Parser(const Packet *in, Packet *out){
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
}

void wdcb(const ChannelIndex idx, const ChannelStatus s, uintptr_t *cntx){
    INFO("Channel %d new state is %d", idx, s);
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
            
            appInitialized = (initializeMainappMailbox() == 0);
            // set up laser modulation
            //setupLaserModulation();
            //mainappData.modulator = LaserModulatorIfc_Get(Channel_Right);
            //setupLaserCapture();
            //setupFakeWatchdog3();
            
            size_t ch;
            for(ch = 0; ch < Channel_Max; ++ch){
                mainappData.channels[ch] = Channel_Get((ChannelIndex)ch);
                Channel_SetCallback(mainappData.channels[ch], wdcb, NULL);
            }

            if (appInitialized)
            {
                INFO("Main App started!");
                mainappData.state = MAINAPP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case MAINAPP_STATE_SERVICE_TASKS:
        {
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
                    notSupported->pLen = 3;
                    notSupported->payload = malloc(3);
                    memcpy(notSupported->payload, &(p->msgID), sizeof(uint16_t));
                    notSupported->payload[2] = p->cmd;
                    SendPacketToBle(MSG_SRC_MAIN, notSupported);
                } else {
                    DEBUG("Parsing command ID 0x%02X", p->cmd);
                    Packet *reply = PACKET_CreateForReply(p);
                    parsers[parserIndex](p, reply);
                    SendPacketToBle(MSG_SRC_MAIN, reply);
                }
                PACKET_Free(p); // really important
            }
            break;
        }

        case POSITIONING_PHASE:
            positioningPhase();
            break;

        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}
