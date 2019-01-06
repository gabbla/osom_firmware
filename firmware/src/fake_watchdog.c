#include "fake_watchdog.h"

// Both left and right using TMR4 for now
#define RIGHT_IVECTOR   _TIMER_4_VECTOR
#define RIGHT_IRQ       19
#define LEFT_IVECTOR    -1
#define LEFT_IRQ        -1

FakeWatchdog dogs[] = {
    {
        .TxCON  = (uint32_t *)0xBF800C00, // T4CON
        .PRx    = (uint32_t *)0xBF800C20, // PR4
        .TMRx   = (uint32_t *)0xBF800C10, // TMR4
        .IFSx   = (uint32_t *)0xBF881030, // IFS0
        .IECx   = (uint32_t *)0xBF881060, // IEC0
        .IPCx   = (uint32_t *)0xBF8810D0, // IPC4
        .enable = false,
        .initialized = false,
        .fake_wd_callback = NULL,
        .ivector = RIGHT_IVECTOR,
        .irq = RIGHT_IRQ
    },
    {
        .TxCON  = (uint32_t *)0xBF800C00,
        .PRx    = (uint32_t *)0xBF800C20,
        .IFSx   = (uint32_t *)0xBF881030,
        .IECx   = (uint32_t *)0xBF881060,
        .IPCx   = (uint32_t *)0xBF8810D0,
        .enable = false,
        .initialized = false,
        .fake_wd_callback = NULL,
        .ivector = LEFT_IVECTOR,
        .irq = LEFT_IRQ
    }
};

// Callbacks
void __ISR(RIGHT_IVECTOR, single) fakewd_right() {
    INFO("!!!!! OBSTACLE RIGHT !!!!!");

    FakeWatchdog *p = FakeWD_Get(FakeWD_Right);
    if(p && p->fake_wd_callback)
        p->fake_wd_callback();

    *(p->IFSx) &= ~(1 << p->irq);
}

//void __ISR(19, single) fakewd_left() {
//    INFO("!!!!! OBSTACLE LEFT !!!!!");
//    
//    FakeWatchdog *p = FakeWD_Get(FakeWD_Left);
//    if(p && p->fake_wd_callback)
//        p->fake_wd_callback();
//
//    *(p->IFSx) &= ~(1 << p->irq);
//}

FakeWatchdog *FakeWD_Get(const FakeWDIndex index){
    if(index > FakeWD_Max)
        return NULL;
    return FakeWD_Initialize(index);
}

FakeWatchdog *FakeWD_Initialize(const FakeWDIndex dog){
    FakeWatchdog *p = &dogs[dog];

    if(p->initialized)
        return p;

    *(p->TxCON) &= ~(1 << 15); // turn off the timer
    *(p->TxCON) &= ~(1 << 1); // internal clock source
    *(p->TxCON) &= ~(1 << 3); // this is a 16 bits timer
    *(p->TxCON) &= ~(7 << 4); // Clear prescaler
    *(p->TxCON) |= (3 << 4); // prescaler 1:8

    // Period set to 550 uS
    *(p->PRx) = 2750;

    // Interrupt
    *(p->IFSx) &= ~(1 << p->irq); // Clear the flag
    *(p->IECx) |= (1 << p->irq); // Enable the interrupt
    *(p->IPCx) &= ~(7 << 2); // clear priority
    *(p->IPCx) |= (7 << 2); // Set priority
    
    p->initialized = true;
    
    return p;
}

void FakeWD_Enable(FakeWatchdog *dog, const bool enable){
    if(!dog || !dog->initialized)
        return;
    dog->enable = enable;
    if(enable)
        *(dog->TxCON) |= (1 << 15);
    else
        *(dog->TxCON) &= ~(1 << 15);
}

void FakeWD_Kick(FakeWatchdog *dog){
    *(dog->TMRx) = 0;
}

void FakeWD_SetCallback(FakeWatchdog *dog, fake_wd_callback cb){
    if(!dog || !dog->initialized)
        return;
    dog->fake_wd_callback = cb;
}

