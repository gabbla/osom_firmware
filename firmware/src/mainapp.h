#ifndef _MAINAPP_H
#define _MAINAPP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "logger.h"
#include "mailbox.h"
#include "somparser.h"
#include "cmdUtils.h"
#include "peripheral/ports/plib_ports.h"
#include "channels.h"


#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// LASER COMMANDS
#define LASER_OFF           (0x0000)
#define LASER_ON_DX         (0x0001)
#define LASER_ON_SX         (0x0002)
#define LASER_ON_ALL        (0x0003)
#define MASK_LASER_DX       (0x0001)
#define MASK_LASER_SX       (0x0002)


// Laser stuff
#define LASER_DX            (0x00)
#define LASER_SX            (0x01)

typedef struct {
    PORTS_CHANNEL port;
    PORTS_BIT_POS pin;
} Laser;
// TODO update with real values
static Laser lasers[2] = {
    {
        // Dx
        .port = PORT_CHANNEL_A,
        .pin = PORTS_BIT_POS_0
    },
    {
        // Sx
        .port = PORT_CHANNEL_A,
        .pin = PORTS_BIT_POS_0
    }
};

typedef enum
{
	/* Application's state machine's initial state. */
	MAINAPP_STATE_INIT=0,
	MAINAPP_STATE_SERVICE_TASKS,


} MAINAPP_STATES;

typedef enum {
    SP_IDLE = 0,
    SP_POSITIONING_PHASE,
} SYSTEM_PHASE;

// Command function parser prototype
typedef void (*cmdParserFunction)(const Packet *, Packet *, uintptr_t);


typedef struct
{
    /* The application's current state */
    MAINAPP_STATES state;
    SYS_OBJ_HANDLE commandMailBox; // Here the app receives the commands
    SYSTEM_PHASE phase; 
    
    SYS_TMR_HANDLE batteryInfoTmr;
    // Left / Right
    Channel *channels[Channel_Max];

} MAINAPP_DATA;

void MAINAPP_Initialize ( void );

void MAINAPP_Tasks( void );

void LED_Tasks();

void MSG_Tasks();

void POW_Tasks();

#endif /* _MAINAPP_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

