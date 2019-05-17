#include "nrfapp.h"

NRFAPP_DATA nrfappData;


/*******************************************************************************
  Function:
    void NRFAPP_Initialize ( void )

  Remarks:
    See prototype in nrfapp.h.
 */

void NRFAPP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    nrfappData.state = NRFAPP_STATE_INIT;

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void NRFAPP_Tasks ( void )

  Remarks:
    See prototype in nrfapp.h.
 */

void NRFAPP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( nrfappData.state )
    {
        /* Application's initial state. */
        case NRFAPP_STATE_INIT:
        {
            bool appInitialized = true;
       
        
            if (appInitialized)
            {
            
                nrfappData.state = NRFAPP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case NRFAPP_STATE_SERVICE_TASKS:
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
