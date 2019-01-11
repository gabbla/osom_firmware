#ifndef CHANNEL_COMMON_H_A8CS3IXR
#define CHANNEL_COMMON_H_A8CS3IXR

#define IS_VALID_CHANNEL_IDX(x)     ((x) < Channel_Max)

typedef enum {
    Channel_Right = 0,
    Channel_Left,
    Channel_Max
} ChannelIndex;



#endif /* end of include guard: CHANNEL_COMMON_H_A8CS3IXR */
