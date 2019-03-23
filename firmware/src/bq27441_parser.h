#ifndef BQ27441_PARSER_H_IZLDRN9M
#define BQ27441_PARSER_H_IZLDRN9M

#include <stdint.h>

typedef enum {
    BQ27441_CONTROL = 0x00,
    BQ27441_TEMPERATURE = 0x02,
    BQ27441_VOLTAGE = 0x04,
    BQ27441_FLAGS =0x06,
    BQ27441_NOMINAL_AVAIL_CAPACITY = 0x08,
    BQ27441_FULL_AVAIL_CAPACITY =0x0A,
    BQ27441_REMAINING_CAPACITY = 0x0C,
    BQ27441_FULL_CHARGE_CAPACITY = 0x0E,
    BQ27441_AVERAGE_CURRENT = 0x10,
    BQ27441_STANDBY_CURRENT = 0x12,
    BQ27441_MAX_LOAD_CURRENT = 0x14,
    BQ27441_AVERAGE_POWER = 0x18,
    BQ27441_STATE_OF_CHARGE = 0x1C,
    BQ27441_INTERNAL_TEMPERATURE = 0x1E,
    BQ27441_STATE_OF_HEALTH = 0x20,
    BQ27441_REMAINING_CAPACITY_UNFILTERED = 0x28,
    BQ27441_REMAINING_CAPACITY_FILTERED = 0x2A,
    BQ27441_FULL_CHARGE_CAPACITY_UNFILTERED = 0x2C,
    BQ27441_FULL_CHARGE_CAPACITY_FILTERED = 0x2E,
    BQ27441_STATE_OF_CHARGE_UNFILTERED = 0x30,
    BQ27441_TRUE_REMAINING_CAPACITY = 0x6A
} BQ27441_Command;

typedef void (*BQ27441_CALLBACK)(BQ27441_Command cmd, uint8_t *buff, size_t len);
typedef uint16_t millivolts_t;
typedef uint16_t soc_t;
typedef uint16_t milliamps_t;

millivolts_t BQ27441_GetMillivolts(const uint8_t *raw) {
    return ((uint16_t)raw[1] << 8 | raw[0]);
}

soc_t BQ27441_GetStateOfCharge(const uint8_t *raw) {
    return ((uint16_t)raw[1] << 8 | raw[0]);
}

milliamps_t BQ27441_GetAverageCurrent(const uint8_t *raw) {
    return ((uint16_t)raw[1] << 8 | raw[0]);
}
#endif /* end of include guard: BQ27441_PARSER_H_IZLDRN9M */
