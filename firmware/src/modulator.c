#include "modulator.h"

static LaserModulatorIfc modulatorIfc[] = {
    {
        .enabled = false,
        .initialized = false,
        .modulator = NULL,
        .enabler = {
            .index  = RIGHT_ENABLER_PORT_IDX,
            .port   = RIGHT_ENABLER_PORT,
            .bitPos = RIGHT_ENABLER_POS
        }
    },
    {
        .enabled = false,
        .initialized = false,
        .modulator = NULL,
        .enabler = {
            .index  = LEFT_ENABLER_PORT_IDX,
            .port   = LEFT_ENABLER_PORT,
            .bitPos = LEFT_ENABLER_POS
        }
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
    PLIB_TMR_PrescaleSelect(tmr, TMR_PRESCALE_VALUE_16);
    PLIB_TMR_Period16BitSet(tmr, 500);

    // Setup the oc
    PLIB_OC_Disable(oc);
    PLIB_OC_TimerSelect(oc, modulator.ocTmrModule);
    PLIB_OC_BufferSizeSelect(oc, OC_BUFFER_SIZE_16BIT);
    PLIB_OC_ModeSelect(oc, OC_COMPARE_PWM_MODE_WITHOUT_FAULT_PROTECTION);
    PLIB_OC_Buffer16BitSet(oc, 250);
    PLIB_OC_PulseWidth16BitSet(oc, 250);

    modulator.initialized = true;
    return &modulator;
}

LaserModulatorIfc *LaserModulatorIfc_Get(const ChannelIndex idx){
    if(!IS_VALID_CHANNEL_IDX(idx))
        return NULL;
    LaserModulatorIfc *p = &modulatorIfc[idx];
    if(p->initialized)
        return p;
    p->modulator = LaserModulator_Intiialize();
    p->initialized = true;
    return p;
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

void ChannelEnabler_Enable(const ChannelEnabler *enabler, const bool enable) {
    if(!enabler)
        return;
    PLIB_PORTS_PinWrite(enabler->index, enabler->port, enabler->bitPos, enable);
}

void LaserModulatorIfc_Enable(LaserModulatorIfc *mod, const bool enable){
    if(!mod || !mod->initialized)
        return;
    mod->enabled = enable;
    LaserModulator_Enable(enable);
    ChannelEnabler_Enable(&mod->enabler, enable);
}
