/*******************************************************************************
 MPLAB Harmony Application Source File

 Company:
 Microchip Technology Inc.

 File Name:
 bleapp.c

 Summary:
 This file contains the source code for the MPLAB Harmony application.

 Description:
 This file contains the source code for the MPLAB Harmony application.  It
 implements the logic of the application's state machine and it may call
 API routines of other MPLAB Harmony modules in the system, such as drivers,
 system services, and middleware.  However, it does not call any of the
 system interfaces (such as the "Initialize" and "Tasks" functions) of any of
 the modules in the system or make any assumptions about when those functions
 are called.  That is the responsibility of the configuration-specific system
 files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
 Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

 Microchip licenses to you the right to use, modify, copy and distribute
 Software only when embedded on a Microchip microcontroller or digital signal
 controller that is integrated into your product or third party product
 (pursuant to the sublicense terms in the accompanying license agreement).

 You should refer to the license agreement accompanying this Software for
 additional information regarding your rights and obligations.

 SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
 MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
 IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
 CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
 OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
 INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
 CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
 SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
 (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END
// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************
#include "bleApp.h"
#include "driver/tmr/src/drv_tmr_local.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

 Summary:
 Holds application data

 Description:
 This structure holds the application's data.

 Remarks:
 This structure should be initialized by the APP_Initialize function.

 Application strings and buffers are be defined outside this structure.
 */

BLEAPP_DATA bleappData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
 */
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

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************
/* TODO:  Add any necessary local functions.
 */

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
// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************
/*******************************************************************************
 Function:
 void BLEAPP_Initialize ( void )

 Remarks:
 See prototype in bleapp.h.
 */

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
/******************************************************************************
 Function:
 void BLEAPP_Tasks ( void )

 Remarks:
 See prototype in bleapp.h.
 */

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
        

		// Initialize the packet queue
		PQUEUE_Init(&bleappData.incoming, MAX_PACKET_IN_QUEUE_IN);
		PQUEUE_Init(&bleappData.outgoing, MAX_PACKET_IN_QUEUE_OUT);

		// Register the HM10 rx buffer
		//registerBuffer();
		reregisterBuffer();

		if (bleappData.hm10 != DRV_HANDLE_INVALID) {
			INFO("BLE App started");
			bleappData.state = BLEAPP_COLLECT_PACKET;
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
		} else if (!PQUEUE_IsEmpty(&bleappData.incoming)) {
			bleappData.state = BLEAPP_STATE_PARSE;
		} else if (!PQUEUE_IsEmpty(&bleappData.outgoing)) {
			bleappData.state = BLEAPP_STATE_REPLY;
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

				Packet p;
				PACKET_Get(bleappData.packet, &p);
				PQUEUE_CODE res = PQUEUE_Enqueue(&bleappData.incoming, &p);
				if (res != PQUEUE_OK) {
					ERROR("Cannot enqueue the packet! %s", PQUEUE_GetErrorStr(res));
				}
				DEBUG("Packet(s) in queue: %d\n", PQUEUE_GetSize(&bleappData.incoming));
				PACKET_Free(&p);
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

	case BLEAPP_STATE_PARSE: {
		Packet packet;
		if (PQUEUE_Dequeue(&bleappData.incoming, &packet) == PQUEUE_OK) {
			DEBUG("Parsing command 0x%02x", packet.cmd);

			Packet reply;
			PACKET_Init(&reply);
			reply.msgID = PACKET_GetMessageId(&packet);

			switch (PACKET_GetCommand(&packet)) {
			case BLE_CMD_PING:
				// Simply reply the message
				reply.cmd = PING_OK;
                // TEST
                //SYS_MSG_OBJECT msgTest;
                //msgTest.nMessageTypeID = LASER_MSG_ID;
                //msgTest.nSource = 1;
                //msgTest.param1 = 1234;
                //SYS_MSG_RESULTS myRes = SYS_MSG_MessageSend(LASER_MAILBOX, &msgTest);
                //if(myRes != SYS_MSG_SENT)
                //    ERROR("Failed to send!! %d", myRes);
				break;

            case BLE_CMD_MODE:
                // TODO forward to slave
                
                break;

			default:
				WARN("Unknown command 0x%02x", packet.cmd);
				break;
			}

			PQUEUE_Enqueue(&bleappData.outgoing, &reply);
			PACKET_Free(&packet);
			PACKET_Free(&reply);
		} else
            WARN("Parser dequeue fail");
		bleappData.state = BLEAPP_STATE_IDLE;
		break;
	}

	case BLEAPP_STATE_REPLY: {
		Packet packet;
		if (PQUEUE_Dequeue(&bleappData.outgoing, &packet) == PQUEUE_OK) {
			size_t size = PACKET_BASE_LEN + packet.pLen;
			DEBUG("Sending a packet long %d bytes", size);
			uint8_t byteArray[size];
			PACKET_GetByteArray(&packet, byteArray);
			DRV_USART_BUFFER_HANDLE txHandler;
			DRV_USART_BufferAddWrite(bleappData.hm10, &txHandler, byteArray, size);
			if (DRV_USART_BUFFER_HANDLE_INVALID == txHandler) {
				WARN("Invalid txHandler!");
//			PQUEUE_CODE enqueueResult;
//			if((enqueueResult = PQUEUE_Enqueue(&bleappData.outgoing, &packet)) != PQUEUE_OK){
//				ERROR("Packet enqueue failed: %s\n", PQUEUE_GetErrorStr(enqueueResult));
//			}
			}

			PACKET_Free(&packet);
		} else 
            WARN("Sender dequeue fail");
		bleappData.state = BLEAPP_STATE_IDLE;
		break;
	}

	default: {
		/* TODO: Handle error in application's state machine. */
		break;

	}
	}
}
/*******************************************************************************
 End of File
 */
