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
#include "peripheral/ports/plib_ports.h"

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

	/* TODO: Define states used by the application state machine. */

} MAINAPP_STATES;


typedef struct
{
    /* The application's current state */
    MAINAPP_STATES state;
    SYS_OBJ_HANDLE laserCmd; // Here the app receives the commands
    SYS_OBJ_HANDLE mainCmd; // Here the app receives the commands

    /* TODO: Define any additional data used by the application. */

} MAINAPP_DATA;

void MAINAPP_Initialize ( void );

void MAINAPP_Tasks( void );


#endif /* _MAINAPP_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

