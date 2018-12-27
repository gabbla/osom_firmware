#include "mainapp.h"

MAINAPP_DATA mainappData;

// TODO think about move this stuff in a separate file
/*
 * @brief Enable the laser modulation. setupLaserModulation must be called
 * @param enable true start the modulation, false stop it
 */
void enableLaserModulation(const bool enable) {
    T2CONbits.ON = enable;
    OC1CONbits.ON = enable;
}

/*
 * @brief Setup the laser modulation, must be called at least once
 * @note At the moment the modulation is setted to f=1000Hz dt=50%
 *       Using OC1 and Timer2 peripherals
 */
void setupLaserModulation() {
    // Running 1KHz @ 50% duty cycle 
    // Setup timer2
    T2CONbits.ON = 0; // Turn off the timer
    T2CONbits.T32 = 0; // TMRx and TMRy 2 16bits timer
    T2CONbits.TCS = 0; // Internal clock source
    T2CONbits.TCKPS = 7; // Prescaler to 1:256 (40MHz / 256 = 156250Hz)
    PR2 = 156; // Period set to 1ms

    // Setup OC1
    OC1CONbits.ON = 0;
    OC1CONbits.OC32 = 0; // OC 16bits mode
    OC1CONbits.OCTSEL = 0; // Use timer 2
    OC1CONbits.OCM = 6; // PWM mode w/o fault
    OC1R = 78;
    OC1RS = 78;
}

/*
 * @brief Power on or off the given laser(s)
 * @param which Laser to be turned controlled, can be ORed (LASER_DX | LASER_SX)
 * @param power state to assign to the laser(s)
 */
void enableLaser(const uint8_t which, const bool power){
    size_t i;
    Laser *laser;
    for(i = 0; i < 2; ++i){
        laser = &lasers[i];
        if(i & which) 
            PLIB_PORTS_PinWrite(PORTS_ID_0, laser->port, laser->pin, power);
    }
    // start or stop the modulation
    enableLaserModulation(power);
}

void nextState(MAINAPP_STATES next){
    mainappData.state = next;
}

void positioningPhase(){
    // TODO manage the input states (I guess something getWatchdogDx status)
    static bool t = true;
    if(t){
        DEBUG("Sending fake status");
        Packet *reply = PACKET_CreatePositionStatus(false, true);
        SendPacketToBle(MSG_SRC_MAIN, reply );
        t = false;
    }
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
            setupLaserModulation();
        
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
