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
#include "bleapp.h"
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
inline void unregisterBuffer(); //Forward declaration
void rxPacketTimeout(uintptr_t context, uint32_t currTick) {
	// To avoid task block, reset the buffer
	WARN("%s()\n", __func__);
	unregisterBuffer();
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************
/* TODO:  Add any necessary local functions.
 */

void restartApp(const char *reason) {
	SYS_DEBUG_PRINT(SYS_ERROR_WARNING, "Resetting BLEAPP. Reason: %s\n", reason);
	bleappData.state = BLEAPP_STATE_INIT;
}

/**
 * @brief Register a buffer to HM10 UART, restart the app if something wrong
 */
inline void registerBuffer() {
	DEBUG("Registering buffer...\n");
	DRV_USART_BufferAddRead(bleappData.hm10, &bleappData.packetHandler, bleappData.packet, MAX_PACKET_LEN);
	if (DRV_USART_BUFFER_HANDLE_INVALID == bleappData.packetHandler) {
		restartApp("Error while registering the packet buffer!");
	} else {
		DEBUG("Buffer registered!\n");
	}
}

inline void unregisterBuffer() {
	DRV_USART_BufferRemove(bleappData.packetHandler);
	bleappData.packetHandler = DRV_USART_BUFFER_HANDLE_INVALID; // FIXME why I should make it invalid? is there a function that do it for me?
}

void APP_MyBufferEventHandler( DRV_USART_BUFFER_EVENT event,
                                   DRV_USART_BUFFER_HANDLE bufferHandle,
                                   uintptr_t context ){
	DEBUG("%s()\n", __func__);
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

		// TODO: why mcp handler == DRV_HANDLE_INVALID but it still works? && bleappData.mcp2200 != DRV_HANDLE_INVALID)
		DEBUG("MCP UART handle is valid? %d\n",
				bleappData.mcp2200 != DRV_HANDLE_INVALID);

		// Initialize the packet queue
		PQUEUE_Init(MAX_PACKET_IN_QUEUE);

		if (bleappData.hm10 != DRV_HANDLE_INVALID) {
			INFO("BLE App started\n");
			bleappData.state = BLEAPP_COLLECT_PACKET;
		} else {
			ERROR("HM10 handler is invalid!\n");
			// TODO reset PIC
		}
		break;
	}

	case BLEAPP_COLLECT_PACKET: {

		if (bleappData.packetHandler == DRV_USART_BUFFER_HANDLE_INVALID) {
			registerBuffer();
		}

		size_t processedSize =
				DRV_USART_BufferCompletedBytesGet(bleappData.packetHandler);

		// If we receive a corrupted packet, we will stuck, so let's add a timeout
		if (processedSize && bleappData.packetTimeout == SYS_TMR_HANDLE_INVALID) {
			// register the callback
			bleappData.packetTimeout =
					SYS_TMR_CallbackSingle(PACKET_RX_TIMEOUT, NULL, rxPacketTimeout);
		}

		if (processedSize >= PACKET_BASE_LEN
				&& processedSize
						== (PACKET_BASE_LEN + bleappData.packet[FIELD_PKTLEN])) {
			// We may have a valid packet, that won't stuck the app aat this stage, we can deregister the callback
			SYS_TMR_CallbackStop(bleappData.packetTimeout);
			bleappData.packetTimeout = SYS_TMR_HANDLE_INVALID; // FIXME why I should make it invalid? is there a function that do it for me?

			size_t payLen = bleappData.packet[FIELD_PKTLEN];
			DEBUG("Packet length: %d bytes, Payload length: %d bytes\n", processedSize, payLen);
			size_t i;
			for (i = 0; i < processedSize; i++)
				SYS_DEBUG_PRINT(SYS_ERROR_DEBUG,"0x%01x ", bleappData.packet[i]);SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "\n");

//			DRV_USART_BUFFER_HANDLE t;
//			DRV_USART_BufferAddWrite(bleappData.hm10, &t, "OK!!", 4);

			if (PACKET_IsRawValid(bleappData.packet)) {
				Packet p;
				PACKET_Get(bleappData.packet, &p);
				PQUEUE_CODE res = PQUEUE_Enqueue(&p);
				if (res != PQUEUE_OK) {
					ERROR("Cannot enqueue the packet! %s\n", PQUEUE_GetErrorStr(res));
				}DEBUG("Packet(s) in queue: %d\n", PQUEUE_GetSize());
				PACKET_Free(&p);
			} else {
				ERROR("The received packet is not valid\n");
			}

			unregisterBuffer();
		}
		break;
	}

		/* TODO: implement your application state machine.*/

		/* The default state should never be executed. */
	default: {
		/* TODO: Handle error in application's state machine. */
		break;

	}
	}
}
/*******************************************************************************
 End of File
 */
