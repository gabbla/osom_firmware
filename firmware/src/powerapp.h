#ifndef _POWERAPP_H
#define _POWERAPP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "bq27441_parser.h"
#include "logger.h"
#include "system_config.h"
#include "system_definitions.h"

#define GAUGE_ADDRESS (0xAA)

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************
//
// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum {
    /* Application's state machine's initial state. */
    POWERAPP_STATE_INIT = 0,
    POWERAPP_STATE_SERVICE_TASKS,

    /* TODO: Define states used by the application state machine. */
    POWERAPP_STATE_IDLE = 9,
    POWERAPP_STATE_WAIT = 10,
    POWERAPP_STATE_ERROR = 11,
    POWERAPP_STATE_GET_DATA = 15,
} POWERAPP_STATES;

typedef enum { REQ_FREE = 0, REQ_SERVED, REQ_PENDING } BQ_Request_Status;

// This struct describes the battery
typedef struct {
    soc_t soc;
} Battery;

typedef struct {
    BQ_Request_Status status;
    BQ27441_CALLBACK bqCallback;
    uintptr_t user_data;
    uint8_t *bqCommand[2];
    uint8_t *bqReply[2];
    BQ27441_Command bqCmdId;
    DRV_I2C_BUFFER_HANDLE hBuff;
} BQ_Request;

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */
#define MAX_BQ_REQUESTS 10
typedef struct {
    /* The application's current state */
    POWERAPP_STATES state;
    DRV_HANDLE gauge;
    BQ_Request requests[MAX_BQ_REQUESTS];
    BQ_Request *currentRequest;
    bool operationInProgress;
    
    Battery battery;
    
    /* TODO: Define any additional data used by the application. */

} POWERAPP_DATA;

bool BQ27441_GetData(const BQ27441_Command cmd, BQ27441_CALLBACK cb,
                     uintptr_t user_data);

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
 */

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void POWERAPP_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    POWERAPP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void POWERAPP_Initialize(void);

/*******************************************************************************
  Function:
    void POWERAPP_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    POWERAPP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void POWERAPP_Tasks(void);

#endif /* _POWERAPP_H */

// DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
// DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

