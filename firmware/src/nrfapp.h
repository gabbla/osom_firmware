#ifndef _NRFAPP_H
#define _NRFAPP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif

typedef enum
{
	/* Application's state machine's initial state. */
	NRFAPP_STATE_INIT=0,
	NRFAPP_STATE_SERVICE_TASKS,

	/* TODO: Define states used by the application state machine. */

} NRFAPP_STATES;



typedef struct
{
    /* The application's current state */
    NRFAPP_STATES state;

    /* TODO: Define any additional data used by the application. */

} NRFAPP_DATA;


void NRFAPP_Initialize ( void );

void NRFAPP_Tasks( void );


#endif /* _NRFAPP_H */

#ifdef __cplusplus
}
#endif
