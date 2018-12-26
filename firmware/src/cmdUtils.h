#ifndef CMDUTILS_H_PAFSTZ6W
#define CMDUTILS_H_PAFSTZ6W

// BLE_CMD_MODE
#define MODE_LASER_MASK     (0x0F)

inline uint8_t MODE_GetMode(const uint8_t *payload) {
    return payload[0] >> 4;
}

inline uint8_t MODE_GetLaser(const uint8_t *payload) {
    return payload[0] & MODE_LASER_MASK;
}

// Positioning stuff
#define POS_LASER_MASK      (0x03)
inline uint8_t POS_GetLaser(const uint8_t *payload) {
    return payload[0] & POS_LASER_MASK;
}

#endif /* end of include guard: CMDUTILS_H_PAFSTZ6W */
