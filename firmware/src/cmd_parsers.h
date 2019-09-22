#ifndef CMD_PARSERS_H_6DOUM9YR
#define CMD_PARSERS_H_6DOUM9YR

#include "logger.h"
#include "somparser.h"

// Command function parser prototype
typedef void (*cmdParserFunction)(const Packet *, Packet *, uintptr_t);

cmdParserFunction CMD_PARSER_GetFunction(const BLECommand cmd);

/*
 * How to add a parser:
 *  1) Add the prototyope here (must match cmdParserFunction)
 *  2) Add the address of the function in the parsers array defined in
 *      cmd_parsers.c
 *  3) Implement the function in mainapp.c
 * 
 * This mess is made to keep sources clean, since the parsers array is full of
 * NULLs
 */

void BLE_CMD_MODE_Parser(const Packet *in, Packet *out, uintptr_t context);

void BLE_CMD_START_POS_Parser(const Packet *in, Packet *out,
                              uintptr_t context);

void BLE_CMD_DONE_POS_Parser(const Packet *in, Packet *out, uintptr_t context);

void BLE_CMD_GET_CHANNEL_STS_Parser(const Packet *in, Packet *out,
                                    uintptr_t context);

#endif /* end of include guard: CMD_PARSERS_H_6DOUM9YR */
