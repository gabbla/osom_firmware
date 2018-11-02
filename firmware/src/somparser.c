/*
 * somparser.c
 *
 *  Created on: 26 ott 2018
 *      Author: gabbla
 */
#include "somparser.h"

uint8_t PACKET_IsRawValid(const uint8_t *raw) {
	return ((raw[FIELD_PREAMBLE0] == PREAMBLE0_VAL)
			&& (raw[FIELD_PREAMBLE1] == PREAMBLE1_VAL));
}

void PACKET_Init(Packet *p){
	SYS_ASSERT(p != NULL, "Packet is null");
	static const uint8_t preamble[] = {PREAMBLE0_VAL, PREAMBLE1_VAL};
	memcpy(p->preamble, preamble, sizeof(preamble));
	p->pLen = 0;
	p->payload = NULL;
}

void PACKET_Get(const uint8_t *raw, Packet *p) {
	memcpy(p, raw, PACKET_BASE_LEN);
	if (p->pLen) {
		p->payload = malloc(p->pLen);
		memcpy((void*) p->payload, &raw[PAYLOAD_START], p->pLen); // FIXME FIELD_PKTLEN may be wrong
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
    SYS_ASSERT(p != NULL, "Packet is null");
    if(p->pLen)
        free(p->payload);
}

uint16_t PACKET_GetMessageId(const Packet *p){
	SYS_ASSERT(p != NULL, "Packet is null");
	return p->msgID;
}
uint8_t PACKET_GetCommand(const Packet *p){
	SYS_ASSERT(p != NULL, "Packet is null");
	return p->cmd;
}

void PACKET_GetByteArray(const Packet *p, uint8_t byteArray[]){
	SYS_ASSERT(p != NULL, "Packet is null");
	memcpy((void*)byteArray, (void*)p, PACKET_BASE_LEN);
	if(p->pLen)
		memcpy(&byteArray[PAYLOAD_START], p->payload, p->pLen);
}

//TODO check on null queue
PQUEUE_CODE PQUEUE_Init(PacketQueue *queue, const size_t capacity) {
	// Mutex initialization
	OSAL_MUTEX_Create(&queue->mutex);
	queue->head = 0;
	queue->tail = 0;
	queue->count = 0;
	queue->capacity = capacity;
	queue->queue = malloc(capacity * sizeof(Packet));
	if (!queue->queue)
		return PQUEUE_NO_MEM;
	return PQUEUE_OK;
}

void PQUEUE_Free(PacketQueue *queue) {
	free(queue->queue);
	OSAL_MUTEX_Delete(&queue->mutex);
	memset(&queue, 0, sizeof(PacketQueue));
}

PQUEUE_CODE PQUEUE_Enqueue(PacketQueue *queue, const Packet *p) {
	if (OSAL_MUTEX_Lock(&queue->mutex, 1000) == OSAL_RESULT_TRUE) {
		if (queue->count == queue->capacity)
			return PQUEUE_FULL; // Overflow
		Packet *dst = &queue->queue[queue->tail];
		PACKET_CODE copyRes = copyPacket(p, dst);
		queue->tail = (queue->tail + 1) % queue->capacity;
		queue->count++;
		OSAL_MUTEX_Unlock(&queue->mutex);
		if (copyRes != PACKET_OK)
			return PQUEUE_NO_MEM;
		return PQUEUE_OK;
	}
	return PQUEUE_FAIL;
}

PQUEUE_CODE PQUEUE_Dequeue(PacketQueue *queue, Packet *p) {
	if (OSAL_MUTEX_Lock(&queue->mutex, 1000) == OSAL_RESULT_TRUE) {
		if (queue->count == 0)
			return PQUEUE_EMPTY; // Empty
		Packet *src = &queue->queue[queue->head];
		PACKET_CODE copyRes = copyPacket(src, p);
		PACKET_Free(src);
		queue->head = (queue->head + 1) % queue->capacity;
		queue->count--;
		OSAL_MUTEX_Unlock(&queue->mutex);
		if (copyRes != PACKET_OK)
			return PQUEUE_FAIL;
		return PQUEUE_OK;
	}
	return PQUEUE_FAIL;
}

size_t PQUEUE_GetSize(PacketQueue *queue) {
	return queue->count;
}

uint8_t PQUEUE_IsFull(PacketQueue *queue) {
	return queue->count == queue->capacity;
}

uint8_t PQUEUE_IsEmpty(PacketQueue *queue) {
	return queue->count == 0;
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
