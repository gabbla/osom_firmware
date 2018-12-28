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
 * @note At the moment the modulation is setted to f=2000Hz dt=50%
 *       Using OC1 and Timer2 peripherals
 */
void setupLaserModulation() {
    // Running 2KHz @ 50% duty cycle 
    // Setup timer2
    T2CONbits.ON = 0; // Turn off the timer
    T2CONbits.T32 = 0; // TMRx and TMRy 2 16bits timer
    T2CONbits.TCS = 0; // Internal clock source
    T2CONbits.TCKPS = 3; // Prescaler to 1:8
    PR2 = 2500; // Period set to 0.5mS

    // Setup OC1
    OC1CONbits.ON = 0;
    OC1CONbits.OC32 = 0; // OC 16bits mode
    OC1CONbits.OCTSEL = 0; // Use timer 2
    OC1CONbits.OCM = 6; // PWM mode w/o fault
    OC1R = 1250; 
    OC1RS = 1250;
}

void __ISR(_INPUT_CAPTURE_4_VECTOR, single) leftInt(){
    // apparently this dummy read is necessesary
    volatile uint32_t dummy = IC4BUF;
    if(dummy < 0xF)
        TMR4 = 0; // watchdog kick
    IFS0bits.IC4IF = 0; // Clear the flag
}

void setupLaserCapture() {
    // Using timer 2 as timebase
    // Left (test only) use IC4
    IC4CONbits.ON = 0; // turn off the module
    asm("NOP");
    IC4CONbits.C32 = 0; // using 16bit timer
    IC4CONbits.ICTMR = 1; // using timer 2
    IC4CONbits.ICI = 0; // interrupt evry capture
    IC4CONbits.ICM = 1; // edge detection mode

    // Enable interrupt
    IFS0bits.IC4IF = 0; // Clear the flag
    IEC0bits.IC4IE = 1; // Enable the interrupt
    IPC4bits.IC4IP = 7; //High priority

    IC4CONbits.ON = 1; // let's go
}

void __ISR(_TIMER_4_VECTOR, ipl7) watchdog3() {
    INFO("!!!!! OBSTACLE !!!!!");
    IFS0bits.T4IF = 0; // Clear the flag
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
    IPC4bits.IC4IP = 7; //High priority

    T4CONbits.ON = 1; // Turn on the timer
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

    setupFakeWatchdog3();
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
            setupLaserCapture();
        
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
