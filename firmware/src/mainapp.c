#include "mainapp.h"

MAINAPP_DATA mainappData;

/*
 * @brief Turn on the given laser(s)
 * @param which Laser to be turned on, can be ORed (LASER_DX | LASER_SX)
 */
void turnOnLaser(uint8_t which){
    size_t i;
    Laser *laser;
    for(i = 0; i < 2; ++i){
        laser = &lasers[i];
        if(i & which)
            PLIB_PORTS_PinSet(PORTS_ID_0, laser->port, laser->pin);
    }
}

/*
 * @brief Turn off the given laser(s)
 * @param which Laser to be turned off, can be ORed (LASER_DX | LASER_SX)
 */
void turnOffLaser(uint8_t which){
    size_t i;
    Laser *laser;
    for(i = 0; i < 2; ++i){
        laser = &lasers[i];
        if(i & which)
            PLIB_PORTS_PinClear(PORTS_ID_0, laser->port, laser->pin);
    }
}

void mainCommandCallback(SYS_MSG_OBJECT *pMessage) {
    DEBUG("New laser command received");
    DEBUG("   Source: %d", pMessage->nSource);
    DEBUG("   Len: %d", pMessage->nSizeData);
    DEBUG("   Dummy: %d", pMessage->dummy);
    char *data = (char *)pMessage->pData;
    size_t i;
    for (i = 0; i < pMessage->nSizeData; ++i) {
        DEBUG("   data[%d] => %d", i, data[i]);
    }
    turnOnLaser(LASER_DX | LASER_SX);
}

int8_t initializeMainappMailbox(){
    mainappData.mainCmd = SYS_MSG_MailboxOpen(
            MAIN_MAILBOX,
            &mainCommandCallback 
    );
    if(mainappData.mainCmd == SYS_OBJ_HANDLE_INVALID) {
        ERROR("Failed to open Main command mailbox");
        return -1;
    } else {
        DEBUG("Main command mailbox is open");
    }

    SYS_OBJ_HANDLE msgType = SYS_MSG_TypeCreate(MAIN_MAILBOX, MAIN_MSG_ID, MAIN_MSG_PRIORITY);
    // Add the message type
    if(msgType != SYS_OBJ_HANDLE_INVALID){
        SYS_MSG_MailboxMsgAdd(mainappData.mainCmd, msgType);
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
        
            if (appInitialized)
            {
                INFO("Main App started!");
                mainappData.state = MAINAPP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case MAINAPP_STATE_SERVICE_TASKS:
        {
        
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
