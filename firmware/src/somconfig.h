#ifndef SOMCONFIG_H_V1ILYIPR
#define SOMCONFIG_H_V1ILYIPR

#include <stdint.h>
#include "logger.h"

// Every time a field is added to SOMConfig the version must be incremented
#define SOM_CFG_VERSION (0x01)

// Define to fource config overwrite (useful during debug)
//#define SOM_CFG_FORCE_OVERWRITE

// Address in eeprom
#define SOM_CFG_ADDRESS 0x0000
// Max name char
#define SOM_CFG_NAME_LEN_MAX        14
// Roles
#define SOM_CFG_ROLE_MASTER         1
#define SOM_CFG_ROLE_SLAVE          0
typedef struct {
    // offset in memory
    uint16_t offset;
    // Configuration version
    uint8_t version;
    uint8_t name[SOM_CFG_NAME_LEN_MAX];
    
    union {
        struct {
            unsigned role : 1; // 1 if master, 0 if slave
            unsigned slave_id : 3; // 0..4 unused in master mode
            uint8_t dummy : 4;
        };
        uint8_t value;
    } flags;
} SOMConfig;

bool CFG_IsValid(const SOMConfig *cfg);

void CFG_SetDefault(SOMConfig *cfg);

bool CFG_IsMaster(const SOMConfig *cfg);

#endif /* end of include guard: SOMCONFIG_H_V1ILYIPR */
