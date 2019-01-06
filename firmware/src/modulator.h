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
    LaserModulatorIndex_0,
    LaserModulatorIndex_MAX
}LaserModulatorIndex;

typedef struct {
    bool enabled;
    bool initialized;
    TMR_MODULE_ID tmrModule;
    OC_MODULE_ID ocModule;
    OC_16BIT_TIMERS ocTmrModule;
} LaserModulator;

LaserModulator *LaserModulator_Intiialize(const LaserModulatorIndex idx);
void LaserModulator_Enable(LaserModulator *mod, const bool enable);


#endif /* end of include guard: MODULATOR_H_VSOJXDHF */
