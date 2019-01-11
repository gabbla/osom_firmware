#include "LaserInput.h"

static LaserInput inputs[] = {
    {
        .initialized = false,
        .intSource = INT_SOURCE_EXTERNAL_4,
        .intVector = INT_VECTOR_INT4
    },
    {
        .initialized = false,
        .intSource = INT_SOURCE_EXTERNAL_4,
        .intVector = INT_VECTOR_INT4
    }
};

inline void manageCallback(const ChannelIndex idx) {
    LaserInput *p = LaserInput_Get(idx);
    if(p && p->callback)
        p->callback(idx, p->context);
    PLIB_INT_SourceFlagClear(INT_ID_0, p->intSource);
}

// Callbacks
void __ISR(_EXTERNAL_4_VECTOR, IPL7AUTO) fakewd_right() {
    manageCallback(Channel_Right);
}

LaserInput *LaserInput_Initialize(const ChannelIndex idx){
    if(!IS_VALID_CHANNEL_IDX(idx))
        return NULL;
    LaserInput *p = &inputs[idx];
    if(p->initialized)
        return p;

    PLIB_INT_SourceFlagClear(INT_ID_0, p->intSource);
    PLIB_INT_SourceDisable(INT_ID_0, p->intSource);
    PLIB_INT_VectorPrioritySet(INT_ID_0, p->intVector, INT_PRIORITY_LEVEL7);

    p->initialized = true;
    return p;
}

bool LaserInput_Enable(LaserInput *input, const bool enable){
    if(!input || !input->initialized)
        return false;
    if(enable)
        PLIB_INT_SourceEnable(INT_ID_0, input->intSource);
    else
        PLIB_INT_SourceDisable(INT_ID_0, input->intSource);
    return true;
}

bool LaserInput_IsEnabled(LaserInput *input){
    if(!input || !input->initialized)
        return false;
    return PLIB_INT_SourceIsEnabled(INT_ID_0, input->intSource);
}

bool LaserInput_SetCallback(LaserInput *input, LASER_INPUT_EVENT callback, uintptr_t *context){
    if(!input || !input->initialized)
        return false;

    input->callback = callback;
    input->context = context;
    return true;
}
