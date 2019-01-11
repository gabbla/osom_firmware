#ifndef MODULATOR_H_VSOJXDHF
#define MODULATOR_H_VSOJXDHF

#include <stdbool.h>
#include "peripheral/oc/plib_oc.h"
#include "peripheral/tmr/plib_tmr.h"

// Config
#define MOD_TMR_MODULE  TMR_ID_2
#define MOD_OC_MODULE   OC_ID_1
#define MOD_OC_TMR      OC_TIMER_16BIT_TMR2

typedef enum {
    LaserModulator_Right = 0,
    LaserModulator_Left,
    LaserModulator_MAX
}LaserModulatorIndex;

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

LaserModulatorIfc *LaserModulatorIfc_Intiialize(const LaserModulatorIndex idx);
void LaserModulatorIfc_Enable(LaserModulatorIfc *mod, const bool enable);


#endif /* end of include guard: MODULATOR_H_VSOJXDHF */
