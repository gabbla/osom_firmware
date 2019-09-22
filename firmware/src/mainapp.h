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

#include "eeprom.h"
#include "somconfig.h"
#include "cmd_parsers.h"
#include <osal/osal.h>

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
#define LASER_DX            (0x01)
#define LASER_SX            (0x02)

#define PACKET_QUEUE_POOL   (10)
    
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
    MAINAPP_STATE_IDLE,
	MAINAPP_STATE_LOAD_CFG,
    MAINAPP_STATE_WAIT_CFG,
    MAINAPP_STATE_WRITE_CFG,


} MAINAPP_STATES;

typedef enum {
    SP_IDLE = 0,
    SP_POSITIONING_PHASE,
} SYSTEM_PHASE;

typedef enum {
    LED_STATUS_INVALID = 0,
    LED_STATUS_DISCONNECTED,
    LED_STATUS_CONNECTED,
    LED_STATUS_DISCOVERED,
} LEDStatus;

typedef struct
{
    /* The application's current state */
    MAINAPP_STATES state;
    SYS_OBJ_HANDLE commandMailBox; // Here the app receives the commands
    DRV_HANDLE hEeprom;
    DRV_I2C_BUFFER_HANDLE eepromBuffHandler;
    SYSTEM_PHASE phase; 
    
    // Config stuff
    SOMConfig config;
    uint16_t configOffset;
    OSAL_MUTEX_DECLARE(configMutex);

    QUEUE_OBJECT *packetQueue;
    QUEUE_ELEMENT_OBJECT packetQueuePool[PACKET_QUEUE_POOL];
    
    SYS_TMR_HANDLE batteryInfoTmr;
    // Left / Right
    Channel *channels[Channel_Max];

    // Configuration data
    RunMode runMode;
    uint8_t activeChannels;
    
    // Led status
    LEDStatus ledStatus;
    
    
} MAINAPP_DATA;

void MAINAPP_Initialize ( void );

void MAINAPP_Tasks( void );

void LED_Tasks();

void MSG_Tasks();

void MODE_Tasks();

void POW_Tasks();

void MAINAPP_SetLedStatus(const LEDStatus status);

bool MAINAPP_IsConfigLoaded();
const SOMConfig *MAINAPP_GetConfig();
#endif /* _MAINAPP_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

