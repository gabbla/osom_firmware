#include "bleapp.h"
#include "driver/tmr/src/drv_tmr_local.h"

BLEAPP_DATA bleappData;

void registerBuffer();
void unregisterBuffer(); //Forward declaration
void reregisterBuffer();
void rxPacketTimeout(uintptr_t context, uint32_t currTick) {
	// To avoid task block, reset the buffer
	WARN("%s()", __func__);
//	unregisterBuffer();
//	registerBuffer();
	reregisterBuffer();
}

//void __ISR(_UART_1_VECTOR, IPL2SOFT) rxBTActivity(void){
//	DEBUG("%s()\n", __func__);
//	PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_USART_1_RECEIVE);
//}

void restartApp(const char *reason) {
	SYS_DEBUG_PRINT(SYS_ERROR_WARNING, "Resetting BLEAPP. Reason: %s", reason);
	bleappData.state = BLEAPP_STATE_INIT;
}

/*
 * @breif Get the next message id to assign to a generated message.
 * @return next message id
 */
uint16_t getNextMessageID() {
    static uint16_t msgID = 0;
    return ++msgID;
}

/**
 * @brief Register a buffer to HM10 UART, restart the app if something wrong
 */
void registerBuffer() {
	DEBUG("Registering buffer...");
	DRV_USART_BufferAddRead(bleappData.hm10, &bleappData.packetHandler, bleappData.packet, MAX_PACKET_LEN);
	if (DRV_USART_BUFFER_HANDLE_INVALID == bleappData.packetHandler) {
		restartApp("Error while registering the packet buffer!");
	} else {
		DEBUG("Buffer registered!");
	}
}

void unregisterBuffer() {
	DEBUG("%s()", __func__);
	DRV_USART_BufferRemove(bleappData.packetHandler);
	bleappData.packetHandler = DRV_USART_BUFFER_HANDLE_INVALID; // FIXME why I should make it invalid? is there a function that do it for me?
}

void reregisterBuffer() {
	DRV_USART_BufferRemove(bleappData.packetHandler);
	bleappData.packetHandler = DRV_USART_BUFFER_HANDLE_INVALID; // FIXME why I should make it invalid? is there a function that do it for me?
	DEBUG("Registering buffer...");
	DRV_USART_BufferAddRead(bleappData.hm10, &bleappData.packetHandler, bleappData.packet, MAX_PACKET_LEN);
	if (DRV_USART_BUFFER_HANDLE_INVALID == bleappData.packetHandler) {
		restartApp("Error while registering the packet buffer!");
	} else {
		DEBUG("Buffer registered!");
	}
}

size_t dataReadyFromBLE() {
	size_t cnt = DRV_USART_BufferCompletedBytesGet(bleappData.packetHandler);
	return ((cnt == ((size_t) -1)) ? 0 : cnt);
}

void startPacketGuard() {
	DEBUG("%s()", __func__);
	bleappData.packetTimeout =
			SYS_TMR_CallbackSingle(PACKET_RX_TIMEOUT, NULL, rxPacketTimeout);
}

void stopPacketGuard() {
	DEBUG("%s()", __func__);
	SYS_TMR_CallbackStop(bleappData.packetTimeout);
	bleappData.packetTimeout = SYS_TMR_HANDLE_INVALID; // FIXME why should I make it invalid? is there a function that do it for me? 
}

typedef struct {
    void *buff;
    DRV_USART_BUFFER_HANDLE handler;
} BLEOut;

#define MAX_BLE_OUT_QUEUE 10
static BLEOut out_queue[MAX_BLE_OUT_QUEUE];

void hm10EventHandler(DRV_USART_BUFFER_EVENT event, DRV_USART_BUFFER_HANDLE handle, uintptr_t context) {
    size_t i;
    for(i = 0; i < MAX_BLE_OUT_QUEUE; ++i){
        BLEOut *tmp = &out_queue[i];
        if(tmp->handler == handle){
            free(tmp->buff);
            tmp->buff = NULL;
            break;
        }
    }
    switch(event) {
        case DRV_USART_BUFFER_EVENT_COMPLETE:
            DEBUG("BLEOut index %d sent successfully", i);
            break;
        case DRV_USART_BUFFER_EVENT_ERROR:
            WARN("BLEOut index %d failed to send", i);
            break;
        default:
            break;
    }
}

void bleOutgoingCallback(SYS_MSG_OBJECT *pMessage) {
    DEBUG("Dispatching message for %d", pMessage->nSource);
    Packet *p = (Packet*)pMessage->pData;
    DEBUG("Cmd: 0x%02X TID: 0x%X MID: 0x%X", p->cmd, p->tid, p->mid);
    size_t size = PACKET_GetFullSize(p);
    size_t i, k;
    for(i = 0; i < MAX_BLE_OUT_QUEUE; i++) {
        BLEOut *msg = &out_queue[i];
        if(msg->buff == NULL) {
            DEBUG("%d it's free", i);
            msg->buff = malloc(size);
            PACKET_GetByteArray(p, msg->buff);

            SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "Sending: ");
            for(k = 0; k < size; ++k)
                SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "0x%02X ", ((uint8_t*)msg->buff)[k]);
            SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "\n");

            DRV_USART_BufferAddWrite(bleappData.hm10, &msg->handler, msg->buff, size);
	        if (DRV_USART_BUFFER_HANDLE_INVALID == msg->handler) {
	        	WARN("Invalid txHandler!");
	        }
            PACKET_Free(p); // really important
            break;
        }
    }
}

int8_t initializeBleAppMailbox(){
    bleappData.bleOutgoing = SYS_MSG_MailboxOpen(
            BLEOUT_MAILBOX,
            &bleOutgoingCallback 
    );

    if(bleappData.bleOutgoing == SYS_OBJ_HANDLE_INVALID) {
        ERROR("Failed to open BLE Outgoing Mail Box");
        return -1;
    } else {
        DEBUG("BLE Outgoing Mail Box is open");
    }

    SYS_OBJ_HANDLE msgType = SYS_MSG_TypeCreate(BLEOUT_MAILBOX, BLEOUT_MSG_ID, BLEOUT_MSG_PRIORITY);
    // Add the message type
    if(msgType != SYS_OBJ_HANDLE_INVALID){
        SYS_MSG_MailboxMsgAdd(bleappData.bleOutgoing, msgType);
        DEBUG("Subuscribed to BLE Outgoing");
        return 0;
    }
    WARN("Subscription to BLE Outgoing failed!");
    return -2;
}

void BLEAPP_Initialize(void) {
	/* Place the App state machine in its initial state. */
	bleappData.state = BLEAPP_STATE_INIT;

	// Handlers
	bleappData.hm10 = DRV_HANDLE_INVALID;
	bleappData.mcp2200 = DRV_HANDLE_INVALID;
	bleappData.i2cHandle = DRV_HANDLE_INVALID;

	// Packet stuff
	bleappData.packetHandler = DRV_USART_BUFFER_HANDLE_INVALID;
	bleappData.packetTimeout = SYS_TMR_HANDLE_INVALID;
}


void powercb(BQ27441_Command cmd, uint8_t *data, size_t s, uintptr_t user_data) {
    if(!data) {
        ERROR("Invalid data received!");
        return;
    }
    // FIXME why I have to do this trick to use req in PACKET_Creat.....??
    Packet req = *((Packet*)user_data);
    Packet *reply = PACKET_CreateForReply(&req);
    switch(cmd) {
        case BQ27441_STATE_OF_CHARGE: {
            soc_t soc = BQ27441_GetStateOfCharge(data);
            DEBUG("State of charge: %d %%", soc);
            SendPacketToBle(MSG_SRC_BLE, PACKET_FillBatteryData(reply, cmd, soc));
            break;
        }
        case BQ27441_VOLTAGE: {
            millivolts_t mv = BQ27441_GetMillivolts(data);
            DEBUG("Voltage: %d mV", mv);
            SendPacketToBle(MSG_SRC_BLE, PACKET_FillBatteryData(reply, cmd, mv));
            break;
        }
        case BQ27441_AVERAGE_CURRENT: {
            milliamps_t avg = BQ27441_GetAverageCurrent(data);
            DEBUG("Average current: %d mAh", avg);
            SendPacketToBle(MSG_SRC_BLE, PACKET_FillBatteryData(reply, cmd, avg));
            break;
        }
    }
}

void manageBleAppMessage(Packet *p) {
    BLECommand cmd = p->cmd;
    switch(cmd) {
        case BLE_CMD_GET_BAT_DATA:
            DEBUG("Battery request from ble [0x%02X]", p->payload[0]);
            BQ27441_GetData(p->payload[0], &powercb, (uintptr_t)p);
            break;
        default:
            WARN("Command 0x%X not managed by BLEApp", cmd);
            break;
    }
}

uint8_t dummyData[5] = {5, 4, 56, 2, 77};
void BLEAPP_Tasks(void) {

	/* Check the application's current state. */
	switch (bleappData.state) {
	/* Application's initial state. */
	case BLEAPP_STATE_INIT: {
		bleappData.hm10 =
				DRV_USART_Open(DRV_USART_INDEX_0, DRV_IO_INTENT_READWRITE);
		bleappData.mcp2200 =
				DRV_USART_Open(DRV_USART_INDEX_1, DRV_IO_INTENT_READWRITE);

//		bleappData.i2cHandle = DRV_I2C_Open(DRV_I2C_INDEX_0, DRV_IO_INTENT_READWRITE);

//		bleappData.eeprom.i2cHandle = bleappData.i2cHandle;
//		bleappData.eeprom.address = 0b10100000;
//
//		bleappData.ioexp.i2cHandle = bleappData.i2cHandle;
//		bleappData.ioexp.address = 0x40;
//
//		PCF_BankWrite(&bleappData.ioexp, 0);

		// TODO: why mcp handler == DRV_HANDLE_INVALID but it still works? && bleappData.mcp2200 != DRV_HANDLE_INVALID)
		DEBUG("MCP UART handle is valid? %d", bleappData.mcp2200 != DRV_HANDLE_INVALID);

		// Interrupt
//		PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_USART_1_RECEIVE);

        // Mailbox stuff
        initializeBleAppMailbox();

		

		// Register the HM10 rx buffer
		//registerBuffer();
		reregisterBuffer();

		if (bleappData.hm10 != DRV_HANDLE_INVALID) {
            // hm10 usart event handler
            DRV_USART_BufferEventHandlerSet(bleappData.hm10, &hm10EventHandler, NULL);
			INFO("BLE App started");
			bleappData.state = BLEAPP_COLLECT_PACKET;
		} else {
			ERROR("HM10 handler is invalid!");
			// TODO reset PIC
		}
		break;
	}

	case BLEAPP_COLLECT_PACKET: {

		size_t processedSize = dataReadyFromBLE();

		// If we receive a corrupted packet, we will stuck, so let's add a timeout
		if (processedSize && bleappData.packetTimeout == SYS_TMR_HANDLE_INVALID) {
			// register the callback
			bleappData.packetTimeout =
					SYS_TMR_CallbackSingle(PACKET_RX_TIMEOUT, NULL, rxPacketTimeout);
		}

		if (processedSize >= PACKET_BASE_LEN
				&& processedSize
						== (PACKET_BASE_LEN + bleappData.packet[FIELD_PKTLEN])) {

			size_t payLen = bleappData.packet[FIELD_PKTLEN];
			DEBUG("Packet length: %d bytes, Payload length: %d bytes", processedSize, payLen);
            
			size_t i;
            SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "Receiving: ");
			for (i = 0; i < processedSize; i++)
				SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "0x%02X ", bleappData.packet[i]);
			SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "\n");

			if (PACKET_IsRawValid(bleappData.packet)) {
				// We can unregister the callback
				stopPacketGuard();

				Packet *p = PACKET_Get(bleappData.packet);
                // TODO move this part in a more appropriate section. ie where also nrf message will come
                DEBUG("Received from 0x%02X to 0x%02X", p->src, p->dst);
                DEBUG("Received: TID: 0x%08X MID:0x%08X", p->tid, p->mid);
                // FIXME For now let's ignore the destination, let's assume that every message destination is master
                if(p->cmd < 0x10) {
                    DEBUG("Message for BLEApp");
                    manageBleAppMessage(p);
                    PACKET_Free(p);
                } else {
                    DEBUG("Message for MainApp");
                    // Forward
                    SYS_MSG_OBJECT message;
                    message.nMessageTypeID = MAIN_MSG_ID;
                    message.nSource = MSG_SRC_MAIN;
                    message.nSizeData = sizeof(Packet);
                    message.pData = (uintptr_t*)p;
                    SYS_MSG_RESULTS myRes = SYS_MSG_MessageSend(MAIN_MAILBOX, &message);
                    if(myRes != SYS_MSG_SENT)
                        ERROR("Failed to send!! %d", myRes);
                    // The packet will be free'd by the receiver
                }
				//PQUEUE_CODE res = PQUEUE_Enqueue(&bleappData.incoming, &p);
				//if (res != PQUEUE_OK) {
				//	ERROR("Cannot enqueue the packet! %s", PQUEUE_GetErrorStr(res));
				//}
				//DEBUG("Packet(s) in queue: %d\n", PQUEUE_GetSize(&bleappData.incoming));
				//PACKET_Free(&p);
			} else {
				ERROR("The received packet is not valid");
			}

//			unregisterBuffer();
//			registerBuffer();
			reregisterBuffer();
		}
        
		break;
	}

	case BLEAPP_STATE_DISPATCH: {
        SYS_MSG_OBJECT *next;
        if((next = SYS_MSG_MailboxMessagesGet(bleappData.bleOutgoing)) 
            != NULL){
                    }
        bleappData.state = BLEAPP_STATE_IDLE;
		break;
	}

	default: {
		/* TODO: Handle error in application's state machine. */
		break;

	}
	}
}
