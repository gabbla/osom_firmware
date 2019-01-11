#ifndef LASERINPUT_H_0HZJSDEB
#define LASERINPUT_H_0HZJSDEB

#include <xc.h>
#include <sys/attribs.h>
#include <stdbool.h>
#include <stdint.h>
#include "channel_common.h"
#include "peripheral/int/plib_int.h"

typedef void (*LASER_INPUT_EVENT)(const ChannelIndex, uintptr_t *);

typedef struct {
    bool initialized;
    INT_SOURCE intSource;
    INT_VECTOR intVector;
    // callback
    LASER_INPUT_EVENT callback;
    uintptr_t *context;
} LaserInput;

LaserInput *LaserInput_Get(const ChannelIndex idx);
bool LaserInput_Enable(LaserInput *input, const bool enable);
bool LaserInput_IsEnabled(LaserInput *input);

bool LaserInput_SetCallback(LaserInput *input, LASER_INPUT_EVENT callback, uintptr_t *context);

#endif /* end of include guard: LASERINPUT_H_0HZJSDEB */
