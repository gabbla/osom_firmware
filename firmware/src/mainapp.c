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
    char *data = (char *)pMessage->pData;
    size_t i;
    for (i = 0; i < pMessage->nSizeData; ++i) {
        DEBUG("   data[%d] => %d", i, data[i]);
    }
}

int8_t initializeMainappMailbox(){
    mainappData.commandMailBox = SYS_MSG_MailboxOpen(
            MAIN_MAILBOX,
//            &mainCommandCallback 
              NULL
    );
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
                DEBUG("Cmd: 0x%02X", p->cmd);
                // TODO parse
                PACKET_Free(next->pData); // really important
            }
            break;
        }

        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}
