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
    p->modulator = LaserModulatorIfc_Get(idx);
    // Get the whatchdog
    p->wdog = FakeWD_Get(idx);
    // Get the input
    p->input = LaserInput_Get(idx);

    p->initialized = true;
    return p;
}


void Channel_Enable(Channel *ch, const bool enable){
    if(!ch || !ch->initialized)
        return;
    ch->enabled = enable;
    LaserModulatorIfc_Enable(ch->modulator, enable);
    FakeWD_Enable(ch->wdog, enable);
    LaserInput_Enable(ch->input, enable);
}

ChannelStatus Channel_GetStatus(Channel *ch){
    if(!ch || !ch->initialized || !ch->enabled)
        return ChannelStatusUnknown;
    return ch->status;
}

bool Channel_SetCallback(Channel *ch, CHANNEL_STATUS_EVENT callback, uintptr_t *context){
    if(!ch || !ch->initialized)
        return false;
    ch->callback = callback;
    ch->context = context;
    return true;
}
