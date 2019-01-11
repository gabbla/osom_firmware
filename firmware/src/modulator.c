#include "modulator.h"

static LaserModulatorIfc modulatorIfc[] = {
    {
        .enabled = false,
        .initialized = false,
        .modulator = NULL
    },
    {
        .enabled = false,
        .initialized = false,
        .modulator = NULL
    }
};

static LaserModulator modulator = {
    .enabled = false,
    .initialized = false,
    .tmrModule = MOD_TMR_MODULE, 
    .ocModule = MOD_OC_MODULE,
    .clients = modulatorIfc,
    .clientCnt = 2
};

LaserModulator *LaserModulator_Intiialize(){
    if(modulator.initialized)
        return &modulator;


    TMR_MODULE_ID tmr = modulator.tmrModule;
    OC_MODULE_ID oc = modulator.ocModule;

    // Setup the timer
    PLIB_TMR_Stop(tmr);
    if(PLIB_TMR_ExistsMode32Bit(tmr))
        PLIB_TMR_Mode16BitEnable(tmr);
    PLIB_TMR_ClockSourceSelect(tmr, TMR_CLOCK_SOURCE_PERIPHERAL_CLOCK);
    PLIB_TMR_PrescaleSelect(tmr, TMR_PRESCALE_VALUE_8);
    PLIB_TMR_Period16BitSet(tmr, 2500); // Set to 500us

    // Setup the oc
    PLIB_OC_Disable(oc);
    PLIB_OC_TimerSelect(oc, modulator.ocTmrModule);
    PLIB_OC_BufferSizeSelect(oc, OC_BUFFER_SIZE_16BIT);
    PLIB_OC_ModeSelect(oc, OC_COMPARE_PWM_MODE_WITHOUT_FAULT_PROTECTION);
    PLIB_OC_Buffer16BitSet(oc, 1250);
    PLIB_OC_PulseWidth16BitSet(oc, 1250);

    modulator.initialized = true;
    return &modulator;
}

void LaserModulator_Enable(const bool enable) {
    if(!modulator.initialized)
        return;
    if(enable){
        PLIB_TMR_Start(modulator.tmrModule);
        PLIB_OC_Enable(modulator.ocModule);
    } else {
        size_t i;
        bool clientStatus = true;
        for(i = 0; i < modulator.clientCnt; ++i)
            clientStatus &= modulator.clients[i]->enabled;
        if(!clientStatus) {
            PLIB_TMR_Stop(modulator.tmrModule);
            PLIB_OC_Disable(modulator.ocModule);
        }
    }
}

LaserModulatorIfc *LaserModulatorIfc_Intiialize(const LaserModulatorIndex idx){
    if(idx >= LaserModulator_MAX)
        return NULL;
    LaserModulatorIfc *p = &modulatorIfc[idx];
    if(p->initialized)
        return p;
    p->modulator = LaserModulator_Intiialize();
    p->initialized = true;
    return p;
}

void LaserModulatorIfc_Enable(LaserModulatorIfc *mod, const bool enable){
    if(!mod || !mod->initialized)
        return;
    mod->enabled = enable;
    LaserModulator_Enable(enable);
}
