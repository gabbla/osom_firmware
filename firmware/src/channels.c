#include "channels.h"

static Channel *channels[] = {
    {
        .status = ChannelStatusUnknown,
        .enabled = false,
        .initialized = false,
    }
};


Channel *Channel_Initialize(const ChannelIndex idx){
    if(idx >= ChannelMax)
        return NULL;

    Channel *p = channels[idx];

    // Get the modulator
    p->modulator = LaserModulator_Intiialize(LaserModulatorIndex_0);
    // Get the whatchdog
    p->wdog = FakeWD_Get((FakeWDIndex)idx);

    p->initialized = true;
    return p;
}


void Channel_Enable(Channel *ch, const bool enable){
    if(!ch || !ch->initialized)
        return;
    ch->enabled = enable;
    // FIXME this will mess with 2 channel since the modulator is shared!
    LaserModulator_Enable(ch->modulator, enable);
    FakeWD_Enable(ch->wdog, enable);
}

ChannelStatus Channel_GetStatus(Channel *ch){
    if(!ch || !ch->initialized || !ch->enabled)
        return ChannelStatusUnknown;
    return ch->status;
}

