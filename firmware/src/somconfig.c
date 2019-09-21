#include "somconfig.h"

static const SOMConfig defconfig = {
    .offset = SOM_CFG_ADDRESS,
    .version = SOM_CFG_VERSION,
    .name = "SOM_TestBoard\0",
    .flags = {
        .role = SOM_CFG_ROLE_MASTER
    }
};

bool CFG_IsValid(const SOMConfig *cfg) {
    if(!cfg)
        return false;
#ifdef SOM_CFG_FORCE_OVERWRITE
    // To adoid recursion
    static bool called = false;
    if(!called) {
        called = true;
        return false;
    }
    return true;
#else
    return cfg->version == SOM_CFG_VERSION;
#endif
}


void CFG_SetDefault(SOMConfig *cfg) {
    memcpy(cfg, &defconfig, sizeof(SOMConfig));
}

bool CFG_IsMaster(const SOMConfig *cfg) {
    if(!cfg)
        return false;
    return cfg->flags.role == SOM_CFG_ROLE_MASTER;
}
