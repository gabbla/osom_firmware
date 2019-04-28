#ifndef CHANNEL_COMMON_H_A8CS3IXR
#define CHANNEL_COMMON_H_A8CS3IXR

#include "system/ports/sys_ports.h"

// Configuration
// Fake Watchdog
#define RIGHT_TMR_MODULE        TMR_ID_4
#define RIGHT_TMR_SOURCE        INT_SOURCE_TIMER_4
#define RIGHT_TMR_VECTOR        INT_VECTOR_T4
#define RIGHT_TMR_ISR           _TIMER_4_VECTOR

#define LEFT_TMR_MODULE         TMR_ID_5
#define LEFT_TMR_SOURCE         INT_SOURCE_TIMER_5
#define LEFT_TMR_VECTOR         INT_VECTOR_T5
#define LEFT_TMR_ISR            _TIMER_5_VECTOR
// Laser Input
#define RIGHT_LASER_EXT_INT     INT_SOURCE_EXTERNAL_2
#define RIGHT_LASER_EXT_VEC     INT_VECTOR_INT2
#define RIGHT_LASER_ISR_VECTOR  _EXTERNAL_2_VECTOR

#define LEFT_LASER_EXT_INT      INT_SOURCE_EXTERNAL_3
#define LEFT_LASER_EXT_VEC      INT_VECTOR_INT3
#define LEFT_LASER_ISR_VECTOR   _EXTERNAL_3_VECTOR
// Modulator
#define MOD_TMR_MODULE  TMR_ID_2
#define MOD_OC_MODULE   OC_ID_1
#define MOD_OC_TMR      OC_TIMER_16BIT_TMR2

// Modulator enabler
#define RIGHT_ENABLER_PORT_IDX  PORTS_ID_0
#define RIGHT_ENABLER_PORT      PORT_CHANNEL_C
#define RIGHT_ENABLER_POS       PORTS_BIT_POS_2

#define LEFT_ENABLER_PORT_IDX   PORTS_ID_0
#define LEFT_ENABLER_PORT       PORT_CHANNEL_A
#define LEFT_ENABLER_POS        PORTS_BIT_POS_9

#define IS_VALID_CHANNEL_IDX(x)     ((x) < Channel_Max)

#define CH_IDX_TO_MASK(x)       (1 << (x))

typedef enum {
    Channel_Right = 0,
    Channel_Left,
    Channel_Max
} ChannelIndex;

typedef enum {
    ChannelStatusUnknown = -1,
    ChannelStatusInactive,
    ChannelStatusActive
} ChannelStatus;


#endif /* end of include guard: CHANNEL_COMMON_H_A8CS3IXR */
