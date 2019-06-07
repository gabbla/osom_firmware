#ifndef _NRFAPP_H
#define _NRFAPP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "logger.h"
#include "memory_map.h"

#include <xc.h>
#include <sys/attribs.h>
#include <peripheral/peripheral.h>
#include "nrf_ifc.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif

typedef struct {
    uint8_t reg;
    uint8_t value;
} NRFConfig;

typedef enum
{
	/* Application's state machine's initial state. */
	NRFAPP_STATE_INIT=0,
    NRFAPP_STATE_CONFIG,
	NRFAPP_STATE_SERVICE_TASKS,

	/* TODO: Define states used by the application state machine. */

} NRFAPP_STATES;

typedef struct
{
    /* The application's current state */
    NRFAPP_STATES state;
    // general purpose timer
    SYS_TMR_HANDLE gpTimer;
    
    // Address info
    uint8_t aw_bytes;
    uint8_t pipe0[5];
    uint8_t pipe1[5];
} NRFAPP_DATA;


void NRFAPP_Initialize ( void );

void NRFAPP_Tasks( void );


#endif /* _NRFAPP_H */

#ifdef __cplusplus
}
#endif
