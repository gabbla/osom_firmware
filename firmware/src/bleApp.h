#ifndef _BLEAPP_H
#define _BLEAPP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "somparser.h"
#include "logger.h"
#include "i2c_device.h"
#include "osal/osal_definitions.h"
#include "mailbox.h"


#include "PCF8574.h"

//#include <xc.h>
//#include <sys/attribs.h>
//#include "peripheral/int/plib_int.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif

#define MAX_PACKET_IN_QUEUE_IN	10
#define MAX_PACKET_IN_QUEUE_OUT	10
#define PACKET_RX_TIMEOUT		1000 // ms

typedef enum
{
	/* Application's state machine's initial state. */
	BLEAPP_STATE_INIT=0,
	BLEAPP_STATE_IDLE,
	BLEAPP_COLLECT_PACKET,
	BLEAPP_STATE_DISPATCH

} BLEAPP_STATES;


typedef struct
{
    /* The application's current state */
    BLEAPP_STATES state;

    // UART handlers
    DRV_HANDLE hm10;
    DRV_HANDLE mcp2200;

    // I2C handler
    DRV_HANDLE i2cHandle;

    // I2C Device
    I2CDevice eeprom;
    I2CDevice ioexp;

    // Packet stuff
    uint8_t packet[MAX_PACKET_LEN];
    DRV_USART_BUFFER_HANDLE packetHandler;
    SYS_TMR_HANDLE packetTimeout;

    // mailbox
    SYS_OBJ_HANDLE bleOutgoing; // Laser mailbox
} BLEAPP_DATA;


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
    void BLEAPP_Initialize ( void )

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
    BLEAPP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void BLEAPP_Initialize ( void );


/*******************************************************************************
  Function:
    void BLEAPP_Tasks ( void )

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
    BLEAPP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void BLEAPP_Tasks( void );


#endif /* _BLEAPP_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

