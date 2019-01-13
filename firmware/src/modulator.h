#ifndef MODULATOR_H_VSOJXDHF
#define MODULATOR_H_VSOJXDHF

#include <stdbool.h>
#include "channel_common.h"
#include "peripheral/oc/plib_oc.h"
#include "peripheral/tmr/plib_tmr.h"

typedef struct _modulatorIfc LaserModulatorIfc;

typedef struct {
    bool enabled;
    bool initialized;
    size_t clientCnt;
    TMR_MODULE_ID tmrModule;
    OC_MODULE_ID ocModule;
    OC_16BIT_TIMERS ocTmrModule;
    LaserModulatorIfc *clients[];
} LaserModulator;

struct _modulatorIfc {
    LaserModulator *modulator;
    bool enabled;
    bool initialized;
};

LaserModulatorIfc *LaserModulatorIfc_Get(const ChannelIndex idx);
void LaserModulatorIfc_Enable(LaserModulatorIfc *mod, const bool enable);


#endif /* end of include guard: MODULATOR_H_VSOJXDHF */
