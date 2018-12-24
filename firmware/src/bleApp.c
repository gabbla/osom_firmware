#include "bleApp.h"
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
	bleappData.packetTimeout = SYS_TMR_HANDLE_INVALID; // FIXME why I should make it invalid? is there a function that do it for me?
}

int8_t initializeBleAppMailbox(){
    bleappData.bleOutgoing = SYS_MSG_MailboxOpen(
            BLEOUT_MAILBOX,
//            &mainCommandCallback 
              NULL
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
			INFO("BLE App started");
			bleappData.state = BLEAPP_STATE_IDLE;
		} else {
			ERROR("HM10 handler is invalid!");
			// TODO reset PIC
		}
		break;
	}

	case BLEAPP_STATE_IDLE: {
		if (dataReadyFromBLE() > 0) {
			bleappData.state = BLEAPP_COLLECT_PACKET;
			if (bleappData.packetTimeout == SYS_TMR_HANDLE_INVALID) {
				startPacketGuard();
			}
		} else {
            // Ugly way
			bleappData.state = BLEAPP_STATE_DISPATCH;
		}
		break;
	}
	case BLEAPP_COLLECT_PACKET: {

		size_t processedSize = dataReadyFromBLE();

		// If we receive a corrupted packet, we will stuck, so let's add a timeout
//		if (processedSize && bleappData.packetTimeout == SYS_TMR_HANDLE_INVALID) {
//			// register the callback
//			bleappData.packetTimeout =
//					SYS_TMR_CallbackSingle(PACKET_RX_TIMEOUT, NULL, rxPacketTimeout);
//		}

		if (processedSize >= PACKET_BASE_LEN
				&& processedSize
						== (PACKET_BASE_LEN + bleappData.packet[FIELD_PKTLEN])) {

			size_t payLen = bleappData.packet[FIELD_PKTLEN];
			DEBUG("Packet length: %d bytes, Payload length: %d bytes", processedSize, payLen);
			size_t i;
			for (i = 0; i < processedSize; i++)
				SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "0x%01x ", bleappData.packet[i]);
			SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "\n");

			if (PACKET_IsRawValid(bleappData.packet)) {
				// We can unregister the callback
				stopPacketGuard();

				Packet *p = PACKET_Get(bleappData.packet);
                if(p->cmd < 0x10) {
                    DEBUG("Message for BLEApp");

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
		bleappData.state = BLEAPP_STATE_IDLE;
		break;
	}

	case BLEAPP_STATE_DISPATCH: {

        SYS_MSG_OBJECT *next;
            if((next = SYS_MSG_MailboxMessagesGet(bleappData.bleOutgoing)) 
                != NULL){
            DEBUG("Dispatching message from %d", next->nSource);
            Packet *p = (Packet*)next->pData;
            DEBUG("Cmd: 0x%02X Seq: 0x%02X", p->cmd, p->msgID);
            size_t size = PACKET_GetFullSize(p);
            uint8_t byteArray[size];
            PACKET_GetByteArray(p, byteArray);
            DRV_USART_BUFFER_HANDLE txHandler;
			DRV_USART_BufferAddWrite(bleappData.hm10, &txHandler, byteArray, size);

			if (DRV_USART_BUFFER_HANDLE_INVALID == txHandler) {
				WARN("Invalid txHandler!");
			}

            free(next->pData); // really important
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
