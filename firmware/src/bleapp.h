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
#include "osal/osal_definitions.h"
#include "mailbox.h"
#include "bq27441_parser.h"

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

#define INVALID_MESSAGE_ID      (0)

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

    // Packet stuff
    uint8_t packet[MAX_PACKET_LEN];
    DRV_USART_BUFFER_HANDLE packetHandler;
    SYS_TMR_HANDLE packetTimeout;

    // mailbox
    SYS_OBJ_HANDLE bleOutgoing; // Laser mailbox
} BLEAPP_DATA;


void BLEAPP_Initialize ( void );


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

