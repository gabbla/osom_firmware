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

typedef enum {
    /* Application's state machine's initial state. */
    NRFAPP_STATE_INIT = 0,
    NRFAPP_STATE_CONFIG,
    NRFAPP_STATE_IDLE,

    NRF_STATE_DISCOVERY,    // Master only
    NRF_STATE_WAIT_DISCOVERY,   // Master only
    NRF_STATE_ANNOUNCE,     // Slave only
    NRF_STATE_WAIT_DISCOVERY_ACK, // Slave only
            
    NRFAPP_STATE_PRE_TX,
    NRFAPP_STATE_TX,
    NRFAPP_STATE_PRE_RX,
    NRFAPP_STATE_RX,

    NRFAPP_STATE_PWR_TX,
    NRFAPP_STATE_SEND_PYLOAD,
    NRFAPP_STATE_START_TX

} NRFAPP_STATES;

typedef enum {
    DEVICE_TYPE_MASTER = 0,
    DEVICE_TYPE_SLAVE
} DEVICE_TYPE;


typedef struct {
    uint8_t sn[14];
    bool valid;
} SOM_Device;


typedef struct
{
    /* The application's current state */
    NRFAPP_STATES state;
    // timers
    SYS_TMR_HANDLE gpTimer;
    // Here the app receives the packets
    SYS_OBJ_HANDLE commandMailBox;

    // device type
    uint8_t device_type;

    // Timeouts
    uint32_t discoveryTimeout;
    uint32_t discoveryAckTimeout;
    
    uint8_t discoveryCnt;

    // Master only
    SOM_Device deviceDiscovered[5];
    
    // Slave only
    bool discovered;
    
    // Address info
    uint8_t aw_bytes;
    uint64_t pipe0;
    uint64_t pipe1;
} NRFAPP_DATA;


void NRFAPP_Initialize ( void );

void NRFAPP_Tasks( void );


#endif /* _NRFAPP_H */

#ifdef __cplusplus
}
#endif
