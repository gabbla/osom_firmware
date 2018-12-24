/*
 * somparser.h
 *
 *  Created on: 26 ott 2018
 *      Author: gabbla
 */

#ifndef PARSER_SOMPARSER_H_
#define PARSER_SOMPARSER_H_

#include <stdint.h> // uint8_t ecc
#include <string.h> // memcpy
#include <stdlib.h> // malloc, free
#include "osal/osal.h"  // mutex


// Preamble
#define PREAMBLE0_VAL		(0x53) // DEC 83
#define PREAMBLE1_VAL		(0x4D) // DEC 77

#define PACKET_BASE_LEN 	6
#define MAX_PAYLOAD_LEN		255
#define MAX_PACKET_LEN		(PACKET_BASE_LEN + MAX_PAYLOAD_LEN)

// Byte position
#define FIELD_PREAMBLE0		0
#define FIELD_PREAMBLE1		1
#define FIELD_MIDHIGH		2
#define FIELD_MIDLOW		3
#define FIELD_PKTLEN		4
#define FIELD_COMMAND		5
#define PAYLOAD_START		6

// Command return code
#define PING_OK				(0x00)

typedef enum __attribute__((packed))_commands {
	BLE_CMD_PING 		= 0x00,
	BLE_CMD_GET_SN		= 0x01,
	BLE_CMD_GET_INFO	= 0x02,
    BLE_CMD_ACK         = 0x03,

    BLE_CMD_MODE        = 0x10,
    BLE_CMD_START_GATE  = 0x11,

    BLE_CMD_RUN_RESULTS = 0x20
} BLECommand;

typedef struct _packet {
    uint8_t 	preamble[2];
    uint16_t    msgID;
    uint8_t 	pLen;
    uint8_t 	cmd;
    uint8_t 	*payload;
} Packet;

typedef enum packet_code {
	PACKET_OK = 0,
	PACKET_NO_MEM = -1,
	PACKET_FAIL = -2
}PACKET_CODE;


uint8_t PACKET_IsRawValid(const uint8_t *raw);
void PACKET_Init(Packet *p);

Packet *PACKET_Get(const uint8_t *raw);
Packet *PACKET_Create();
void PACKET_Free(Packet *p);

// Utils

inline uint16_t PACKET_GetMessageId(const Packet *p);
inline BLECommand PACKET_GetCommand(const Packet *p);

void PACKET_GetByteArray(const Packet *p, uint8_t byteArray[]);
size_t PACKET_GetFullSize(const Packet *p);

#endif /* PARSER_SOMPARSER_H_ */
