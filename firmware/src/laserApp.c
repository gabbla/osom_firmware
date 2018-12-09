/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    laserapp.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "laserApp.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

LASERAPP_DATA laserappData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/
void laserCommandCallback(SYS_MSG_OBJECT *pMessage) {

}
// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void LASERAPP_Initialize ( void )

  Remarks:
    See prototype in laserapp.h.
 */

void LASERAPP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    laserappData.state = LASERAPP_STATE_INIT;

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void LASERAPP_Tasks ( void )

  Remarks:
    See prototype in laserapp.h.
 */

void LASERAPP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( laserappData.state )
    {
        /* Application's initial state. */
        case LASERAPP_STATE_INIT:
        {
            volatile bool appInitialized = false;

            // Open the mailbox
            laserappData.laserCmd = SYS_MSG_MailboxOpen(
                    LASER_MAILBOX,
                    &laserCommandCallback
            );
            
            if(laserappData.laserCmd == SYS_OBJ_HANDLE_INVALID) {
                ERROR("Failed to open Laser mailbox\n");
            } else {
                DEBUG("Laser command mailbox is open\n");
                appInitialized = true;
            }

            // Add the message type
            SYS_OBJ_HANDLE msgType = getLaserMessageType();
            if(msgType != SYS_OBJ_HANDLE_INVALID){
                SYS_MSG_MailboxMsgAdd(laserappData.laserCmd, msgType);
                DEBUG("Subuscribed to laser command\n");
            }
                    
            if (appInitialized)
            {
                INFO("Laser App started\n");
                laserappData.state = LASERAPP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case LASERAPP_STATE_SERVICE_TASKS:
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

static const SYS_OBJ_HANDLE getLaserMessageType() {
    static SYS_OBJ_HANDLE msgType = NULL;
    if(!msgType) {
        msgType = SYS_MSG_TypeCreate(LASER_MAILBOX, LASER_MSG_ID, LASER_MSG_PRIORITY);
        if(msgType == SYS_OBJ_HANDLE_INVALID) {
            // error, but propagate the failure
            ERROR("Laser message type creation failed\n");
        }
    }
    return msgType;
}

/*******************************************************************************
 End of File
 */
