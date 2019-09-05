#include "channels.h"

#define LedOn(x) SYS_PORTS_PinSet(x->module_id, x->port, x->bit)
#define LedOff(x) SYS_PORTS_PinClear(x->module_id, x->port, x->bit)

struct _led {
    PORTS_MODULE_ID module_id;
    PORTS_CHANNEL port;
    PORTS_BIT_POS bit;
};

static Led rightStatus = {
    .module_id = PORTS_ID_0, .port = PORT_CHANNEL_C, .bit = PORTS_BIT_POS_0};

static Led leftStatus = {
    .module_id = PORTS_ID_0, .port = PORT_CHANNEL_B, .bit = PORTS_BIT_POS_8};

static Channel channels[] = {
    { // right
        .status = ChannelStatusUnknown,
        .enabled = false,
        .initialized = false,
        .led = &rightStatus
    },
    { // left
        .status = ChannelStatusUnknown,
        .enabled = false,
        .initialized = false,
        .led = &leftStatus
    }
};

// fwd declaration
void Channel_FakeWDCallback(const ChannelIndex idx, uintptr_t *context);
void Channel_LaserInputCallback(const ChannelIndex idx, uintptr_t *context);

Channel *Channel_Get(const ChannelIndex idx){
    if(idx >= Channel_Max)
        return NULL;

    Channel *p = &channels[idx];
    if(p->initialized)
        return p;

    // Get the modulator
    p->modulator = LaserModulatorIfc_Get(idx);
    // Get the whatchdog
    p->wdog = FakeWD_Get(idx);
    // Get the input
    p->input = LaserInput_Get(idx);

    // Register callback to FakeWD
    FakeWD_SetCallback(p->wdog, Channel_FakeWDCallback, NULL);
    LaserInput_SetCallback(p->input, Channel_LaserInputCallback, NULL);

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


// Used internally, fired by watchdog
// If this callback is fired, we are in ChannelStatusInactive for sure
void Channel_FakeWDCallback(const ChannelIndex idx, uintptr_t *context) {
    Channel *c = Channel_Get(idx);
    LedOff(c->led);
    if(c->status != ChannelStatusInactive && c->callback){
        c->status = ChannelStatusInactive;
        c->callback(idx, c->status, c->context);
    }
}

// Used internally, fired by LaserInput
void Channel_LaserInputCallback(const ChannelIndex idx, uintptr_t *context) {
    Channel *c = Channel_Get(idx);
    FakeWD_Kick(c->wdog);
    if(c->status != ChannelStatusActive && c->callback){
        LedOn(c->led);
        c->status = ChannelStatusActive;
        c->callback(idx, c->status, c->context);
    }
}

