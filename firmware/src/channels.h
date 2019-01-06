#ifndef CHANNELS_H_GW2BCLQ3
#define CHANNELS_H_GW2BCLQ3

#include <stdint.h>
#include <stdbool.h>
#include "fake_watchdog.h"

/*
 * A channel is made by:
 *  - modulator (common between the channels)
 *  - input with interrupt
 *  - (fake) watchdog
 */

typedef enum {
    Channel_Right = 0, 
    Channel_Left,
    Channel_MAX
} ChannelIndex;

typedef enum {
    ChannelStatusUnknown = -1,
    ChannelStatusActive,
    ChannelStatusInactive
} ChannelStatus;

typedef struct {
    ChannelStatus status;
    bool enabled;
    bool initialized;
    FakeWatchdog *wdog;
} Channel;

Channel *Channel_Initialize(const ChannelIndex idx);

void Channel_Enable(Channel *ch, const bool enable);
ChannelStatus Channel_GetStatus(Channel *ch);

#endif /* end of include guard: CHANNELS_H_GW2BCLQ3 */
