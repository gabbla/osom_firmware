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
	BLE_CMD_PING 		= 0,
	BLE_CMD_GET_SN		= 1,
	BLE_CMD_GET_INFO	= 2
} BLECommand;

typedef struct _packet {
    uint8_t 	preamble[2];
    uint16_t    msgID;
    uint8_t 	pLen;
    uint8_t 	cmd;
    uint8_t 	*payload;
} Packet;

typedef struct _packetQueue {
	size_t	head;
	size_t 	tail;
	Packet 	*queue;
	size_t	count;
	size_t	capacity;
	OSAL_MUTEX_HANDLE_TYPE mutex;
} PacketQueue;

typedef enum queue_code {
	PQUEUE_OK = 0,
	PQUEUE_NO_MEM = -1,
	PQUEUE_FULL = -2,
	PQUEUE_EMPTY = -3,
	PQUEUE_FAIL = -4
}PQUEUE_CODE;

typedef enum packet_code {
	PACKET_OK = 0,
	PACKET_NO_MEM = -1,
	PACKET_FAIL = -2
}PACKET_CODE;


uint8_t PACKET_IsRawValid(const uint8_t *raw);
void PACKET_Init(Packet *p);
void PACKET_Get(const uint8_t *raw, Packet *p);
void PACKET_Free(Packet *p);

// Utils

inline uint16_t PACKET_GetMessageId(const Packet *p);
inline BLECommand PACKET_GetCommand(const Packet *p);

void PACKET_GetByteArray(const Packet *p, uint8_t byteArray[]);

// -------------

PQUEUE_CODE PQUEUE_Init(PacketQueue *queue, const size_t capacity);
void PQUEUE_Free(PacketQueue *queue);
void PQUEUE_Reset(PacketQueue *queue);
PQUEUE_CODE PQUEUE_Enqueue(PacketQueue *queue, const Packet *p);
PQUEUE_CODE PQUEUE_Dequeue(PacketQueue *queue, Packet *p);
size_t PQUEUE_GetSize(PacketQueue *queue);
uint8_t PQUEUE_IsFull(PacketQueue *queue);
uint8_t PQUEUE_IsEmpty(PacketQueue *queue);

const char* PQUEUE_GetErrorStr(const PQUEUE_CODE code);

#endif /* PARSER_SOMPARSER_H_ */
