#include "modulator.h"

static LaserModulator modulators[] = {
    {
        .enabled = false,
        .initialized = false,
        .tmrModule = MOD_TMR_MODULE, 
        .ocModule = MOD_OC_MODULE
    }
};


LaserModulator *LaserModulator_Intiialize(const LaserModulatorIndex idx){
    if(idx >= LaserModulatorIndex_MAX)
        return NULL;
    
    LaserModulator *p = &modulators[idx];
    if(p->initialized)
        return p;

    TMR_MODULE_ID tmr = p->tmrModule;
    OC_MODULE_ID oc = p->ocModule;

    // Setup the timer
    PLIB_TMR_Stop(tmr);
    if(PLIB_TMR_ExistsMode32Bit(tmr))
        PLIB_TMR_Mode16BitEnable(tmr);
    PLIB_TMR_ClockSourceSelect(tmr, TMR_CLOCK_SOURCE_PERIPHERAL_CLOCK);
    PLIB_TMR_PrescaleSelect(tmr, TMR_PRESCALE_VALUE_8);
    PLIB_TMR_Period16BitSet(tmr, 2500); // Set to 500us

    // Setup the oc
    PLIB_OC_Disable(oc);
    PLIB_OC_TimerSelect(oc, p->ocTmrModule);
    PLIB_OC_BufferSizeSelect(oc, OC_BUFFER_SIZE_16BIT);
    PLIB_OC_ModeSelect(oc, OC_COMPARE_PWM_MODE_WITHOUT_FAULT_PROTECTION);
    PLIB_OC_Buffer16BitSet(oc, 1250);
    PLIB_OC_PulseWidth16BitSet(oc, 1250);

    p->initialized = true;
    return p;
}

void LaserModulator_Enable(LaserModulator *mod, const bool enable){
    if(!mod || !mod->initialized)
        return;

    if(enable){
        PLIB_TMR_Start(mod->tmrModule);
        PLIB_OC_Enable(mod->ocModule);
    } else {
        PLIB_TMR_Stop(mod->tmrModule);
        PLIB_OC_Disable(mod->ocModule);
    }
}
