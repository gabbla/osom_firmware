#ifndef FAKE_WATCHDOG_H_LZUDEQAU
#define FAKE_WATCHDOG_H_LZUDEQAU

#include <xc.h>
#include <sys/attribs.h>
#include <stdbool.h>
#include <stdint.h>
#include "logger.h"

#include "peripheral/ports/plib_ports.h"

typedef void (*fake_wd_callback)(uintptr_t *);

typedef enum {
    FakeWD_Right = 0,
    FakeWD_Left = 1,
    FakeWD_Max
} FakeWDIndex;


typedef struct {
    bool enable;
    bool initialized;
    // Register pointer
    uint32_t *TxCON;
    uint32_t *PRx;
    uint32_t *TMRx;
    uint32_t *IFSx;
    uint32_t *IECx;
    uint32_t *IPCx;
    uint8_t ivector;
    uint8_t _irq;
    // callback
    void (*fake_wd_callback)(uintptr_t *);
    uintptr_t *context;
} FakeWatchdog;

FakeWatchdog *FakeWD_Get(const FakeWDIndex index);

FakeWatchdog *FakeWD_Initialize(const FakeWDIndex dog);
void FakeWD_Enable(FakeWatchdog *dog, const bool enable);
void FakeWD_Kick(FakeWatchdog *dog);
void FakeWD_SetCallback(FakeWatchdog *dog, fake_wd_callback cb, uintptr_t *cntx);

#endif /* end of include guard: FAKE_WATCHDOG_H_LZUDEQAU */
