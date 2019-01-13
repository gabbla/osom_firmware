#ifndef CHANNELS_H_GW2BCLQ3
#define CHANNELS_H_GW2BCLQ3

#include <stdint.h>
#include <stdbool.h>
#include "channel_common.h"
#include "fake_watchdog.h"
#include "modulator.h"
#include "LaserInput.h"

/*
 * A channel is made by:
 *  - modulator (common between the channels)
 *  - input with interrupt
 *  - (fake) watchdog
 */


typedef void (*CHANNEL_STATUS_EVENT)(const ChannelIndex, const ChannelStatus, uintptr_t *);

typedef struct {
    ChannelStatus status;
    bool enabled;
    bool initialized;;
    LaserModulatorIfc *modulator;
    FakeWatchdog *wdog;
    LaserInput *input;
    // Callback
    CHANNEL_STATUS_EVENT callback;
    uintptr_t *context;
} Channel;

Channel *Channel_Get(const ChannelIndex idx);
void Channel_Enable(Channel *ch, const bool enable);
ChannelStatus Channel_GetStatus(Channel *ch);

bool Channel_SetCallback(Channel *ch, CHANNEL_STATUS_EVENT callback, uintptr_t *context);

#endif /* end of include guard: CHANNELS_H_GW2BCLQ3 */
