#ifndef FAKE_WATCHDOG_H_LZUDEQAU
#define FAKE_WATCHDOG_H_LZUDEQAU

#include <xc.h>
#include <sys/attribs.h>
#include <stdbool.h>
#include <stdint.h>
#include "logger.h"
#include "channel_common.h"

#include "peripheral/int/plib_int.h"
#include "peripheral/tmr/plib_tmr.h"


typedef void (*FAKE_WD_EXPIRED)(const ChannelIndex, uintptr_t *);

typedef struct {
    bool enable;
    bool initialized;
    TMR_MODULE_ID tmrModule;
    INT_SOURCE intSource;
    INT_VECTOR intVector;
    // callback
    FAKE_WD_EXPIRED callback;
    uintptr_t *context;
} FakeWatchdog;

FakeWatchdog *FakeWD_Get(const ChannelIndex index);

FakeWatchdog *FakeWD_Initialize(const ChannelIndex dog);
void FakeWD_Enable(FakeWatchdog *dog, const bool enable);
void FakeWD_Kick(FakeWatchdog *dog);
void FakeWD_SetCallback(FakeWatchdog *dog, FAKE_WD_EXPIRED callback, uintptr_t *context);

#endif /* end of include guard: FAKE_WATCHDOG_H_LZUDEQAU */
