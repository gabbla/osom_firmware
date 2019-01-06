#include "channels.h"

static Channel *channels[] = {
    {
        .status = ChannelStatusUnknown,
        .enabled = false,
        .initialized = false,
    }
};

// Modulation prototypes
void setupLaserModulation();
void enableLaserModulation();

Channel *Channel_Initialize(const ChannelIndex idx){
    if(idx >= ChannelMax)
        return NULL;

    // TODO avoid calling this 2 times
    setupLaserModulation();

    Channel *p = channels[idx];
    
    // Get the whatchdog
    p->wdog = FakeWD_Get((FakeWDIndex)idx);

    p->initialized = true;
    return p;
}


void Channel_Enable(Channel *ch, const bool enable){
    if(!ch || !ch->initialized)
        return;
    ch->enabled = enable;

    enableLaserModulation();

    FakeWD_Enable(ch->wdog, enable);
}

ChannelStatus Channel_GetStatus(Channel *ch){
    if(!ch || !ch->initialized || !ch->enabled)
        return ChannelStatusUnknown;
    return ch->status;
}

////////////////////////////////////////////////////////////////////////////////
// Modulator stuff
////////////////////////////////////////////////////////////////////////////////

/*
 *  This function depends on the 2 channel states. If at least one is enable,
 *  the modulation is enabled. To control the modulation simply enable the needed
 *  channel.
 */
void enableLaserModulation() {
    bool enable = channels[Channel_Right]->enabled || channels[Channel_Left]->enabled;
    T2CONbits.ON = enable;
    OC1CONbits.ON = enable;
}

/*
 * @brief Setup the laser modulation, must be called at least once
 * @note At the moment the modulation is setted to f=2000Hz dt=50%
 *       Using OC1 and Timer2 peripherals
 */
void setupLaserModulation() {
    // Running 2KHz @ 50% duty cycle 
    // Setup timer2
    T2CONbits.ON = 0; // Turn off the timer
    T2CONbits.T32 = 0; // TMRx and TMRy 2 16bits timer
    T2CONbits.TCS = 0; // Internal clock source
    T2CONbits.TCKPS = 3; // Prescaler to 1:8
    PR2 = 2500; // Period set to 0.5mS

    // Setup OC1
    OC1CONbits.ON = 0;
    OC1CONbits.OC32 = 0; // OC 16bits mode
    OC1CONbits.OCTSEL = 0; // Use timer 2
    OC1CONbits.OCM = 6; // PWM mode w/o fault
    OC1R = 1250; 
    OC1RS = 1250;
}

