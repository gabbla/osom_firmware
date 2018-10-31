/*
 * somparser.c
 *
 *  Created on: 26 ott 2018
 *      Author: gabbla
 */
#include "somparser.h"

PacketQueue packetQueue;

OSAL_MUTEX_HANDLE_TYPE queueMutex;


uint8_t PACKET_IsRawValid(const uint8_t *raw) {
	return ((raw[FIELD_PREAMBLE0] == PREAMBLE0_VAL)
			&& (raw[FIELD_PREAMBLE1] == PREAMBLE1_VAL));
}

void PACKET_Get(const uint8_t *raw, Packet *p) {
	memcpy(p, raw, PACKET_BASE_LEN);
	if (p->pLen) {
		p->payload = malloc(p->pLen);
		memcpy((void*) p->payload, &raw[FIELD_PKTLEN], p->pLen);
	}
}

PACKET_CODE copyPacket(const Packet *src, Packet *dst) {
	memcpy((void*) dst, (void*) src, PACKET_BASE_LEN);
	if (src->pLen) {
		dst->payload = malloc(dst->pLen);
		if (dst->payload == NULL)
			return PACKET_NO_MEM;
		memcpy((void*) dst->payload, (void*) src->payload, dst->pLen);
	}
	return PACKET_OK;
}

void PACKET_Free(Packet *p) {
	free(p->payload);
}

PQUEUE_CODE PQUEUE_Init(const size_t capacity) {
	// Mutex initialization
	OSAL_MUTEX_Create(&queueMutex);
	packetQueue.head = 0;
	packetQueue.tail = 0;
	packetQueue.count = 0;
	packetQueue.capacity = capacity;
	packetQueue.queue = malloc(capacity * sizeof(Packet));
	if (!packetQueue.queue)
		return PQUEUE_NO_MEM;
	return PQUEUE_OK;
}

void PQUEUE_Free() {
	free(packetQueue.queue);
	memset(&packetQueue, 0, sizeof(PacketQueue));
}

PQUEUE_CODE PQUEUE_Enqueue(const Packet *p) {
	if (OSAL_MUTEX_Lock(&queueMutex, 1000) == OSAL_RESULT_TRUE) {
		if (packetQueue.count == packetQueue.capacity)
			return PQUEUE_FULL; // Overflow
		Packet *dst = &packetQueue.queue[packetQueue.tail];
		PACKET_CODE copyRes = copyPacket(p, dst);
		packetQueue.tail = (packetQueue.tail + 1) % packetQueue.capacity;
		packetQueue.count++;
		OSAL_MUTEX_Unlock(&queueMutex);
		if (copyRes != PACKET_OK)
			return PQUEUE_NO_MEM;
		return PQUEUE_OK;
	}
	return PQUEUE_FAIL;
}

PQUEUE_CODE PQUEUE_Dequeue(Packet *p) {
	if (OSAL_MUTEX_Lock(&queueMutex, 1000) == OSAL_RESULT_TRUE) {
		if (packetQueue.count == 0)
			return PQUEUE_FAIL; // Empty
		Packet *src = &packetQueue.queue[packetQueue.head];
		PACKET_CODE copyRes = copyPacket(src, p);
		packetQueue.head = (packetQueue.head + 1) % packetQueue.capacity;
		packetQueue.count--;
		OSAL_MUTEX_Unlock(&queueMutex);
		if (copyRes != PACKET_OK)
			return PQUEUE_EMPTY;
		return PQUEUE_OK;
	}
	return PQUEUE_FAIL;
}

size_t PQUEUE_GetSize() {
	return packetQueue.count;
}

uint8_t PQUEUE_IsFull() {
	return packetQueue.count == packetQueue.capacity;
}

uint8_t PQUEUE_IsEmpty() {
	return packetQueue.count == 0;
}

const char* PQUEUE_GetErrorStr(const PQUEUE_CODE code) {
	switch (code) {
	case PQUEUE_OK:
		return "OK";
	case PQUEUE_NO_MEM:
		return "No memory";
	case PQUEUE_FULL:
		return "Queue is full";
	case PQUEUE_EMPTY:
		return "Queue is empty";
	case PQUEUE_FAIL:
		return "Fail";
	}

	return "Invalid error code";
}
