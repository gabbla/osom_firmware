/*******************************************************************************
  MPLAB Harmony System Configuration Header

  File Name:
    system_config.h

  Summary:
    Build-time configuration header for the system defined by this MPLAB Harmony
    project.

  Description:
    An MPLAB Project may have multiple configurations.  This file defines the
    build-time options for a single configuration.

  Remarks:
    This configuration header must not define any prototypes or data
    definitions (or include any files that do).  It only provides macro
    definitions for build-time configuration options that are not instantiated
    until used by another MPLAB Harmony module or application.

    Created with MPLAB Harmony Version 2.06
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
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

#ifndef _SYSTEM_CONFIG_H
#define _SYSTEM_CONFIG_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/*  This section Includes other configuration headers necessary to completely
    define this configuration.
*/


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: System Service Configuration
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/* Common System Service Configuration Options
*/
#define SYS_VERSION_STR           "2.06"
#define SYS_VERSION               20600

// *****************************************************************************
/* Clock System Service Configuration Options
*/
#define SYS_CLK_FREQ                        72000000ul
#define SYS_CLK_BUS_PERIPHERAL_1            8000000ul
#define SYS_CLK_UPLL_BEFORE_DIV2_FREQ       96000000ul
#define SYS_CLK_CONFIG_PRIMARY_XTAL         8000000ul
#define SYS_CLK_CONFIG_SECONDARY_XTAL       32768ul
   
/*** Ports System Service Configuration ***/
#define SYS_PORT_A_ANSEL        0xFA6C
#define SYS_PORT_A_TRIS         0xFDFF
#define SYS_PORT_A_LAT          0x0000
#define SYS_PORT_A_ODC          0x0000
#define SYS_PORT_A_CNPU         0x0000
#define SYS_PORT_A_CNPD         0x0000
#define SYS_PORT_A_CNEN         0x0000

#define SYS_PORT_B_ANSEL        0x1C40
#define SYS_PORT_B_TRIS         0x5CEE
#define SYS_PORT_B_LAT          0x0000
#define SYS_PORT_B_ODC          0x0000
#define SYS_PORT_B_CNPU         0x0000
#define SYS_PORT_B_CNPD         0x0000
#define SYS_PORT_B_CNEN         0x0000

#define SYS_PORT_C_ANSEL        0xFC00
#define SYS_PORT_C_TRIS         0xFFCA
#define SYS_PORT_C_LAT          0x0010
#define SYS_PORT_C_ODC          0x0050
#define SYS_PORT_C_CNPU         0x0000
#define SYS_PORT_C_CNPD         0x0000
#define SYS_PORT_C_CNEN         0x0040


/*** Console System Service Configuration ***/

#define SYS_CONSOLE_OVERRIDE_STDIO
#define SYS_CONSOLE_DEVICE_MAX_INSTANCES        2
#define SYS_CONSOLE_INSTANCES_NUMBER            1
#define SYS_CONSOLE_UART_IDX               DRV_USART_INDEX_1
#define SYS_CONSOLE_UART_BAUD_RATE_IDX     DRV_USART_BAUD_RATE_IDX1
#define SYS_CONSOLE_UART_RD_QUEUE_DEPTH    1
#define SYS_CONSOLE_UART_WR_QUEUE_DEPTH    64
#define SYS_CONSOLE_BUFFER_DMA_READY



/*** Debug System Service Configuration ***/
#define SYS_DEBUG_ENABLE
#define DEBUG_PRINT_BUFFER_SIZE       8192
#define SYS_DEBUG_BUFFER_DMA_READY
#define SYS_DEBUG_USE_CONSOLE

/*** Interrupt System Service Configuration ***/
#define SYS_INT                     true
/*** Message System Service Configuration ***/

#define SYS_MSG_MAX_MAILBOXES        4
#define SYS_MSG_MAX_TYPES            2

// *****************************************************************************
/* Random System Service Configuration Options
*/

#define SYS_RANDOM_CRYPTO_SEED_SIZE  32

/*** Timer System Service Configuration ***/
#define SYS_TMR_POWER_STATE             SYS_MODULE_POWER_RUN_FULL
#define SYS_TMR_DRIVER_INDEX            DRV_TMR_INDEX_0
#define SYS_TMR_MAX_CLIENT_OBJECTS      5
#define SYS_TMR_FREQUENCY               1000
#define SYS_TMR_FREQUENCY_TOLERANCE     10
#define SYS_TMR_UNIT_RESOLUTION         10000
#define SYS_TMR_CLIENT_TOLERANCE        10
#define SYS_TMR_INTERRUPT_NOTIFICATION  false

// *****************************************************************************
// *****************************************************************************
// Section: Driver Configuration
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/* I2C Driver Configuration Options
*/
#define DRV_I2C_INTERRUPT_MODE                    		true
#define DRV_I2C_CLIENTS_NUMBER                    		1
#define DRV_I2C_INSTANCES_NUMBER                  		1

#define DRV_I2C_PERIPHERAL_ID_IDX0                		I2C_ID_2
#define DRV_I2C_OPERATION_MODE_IDX0               		DRV_I2C_MODE_MASTER
#define DRV_SCL_PORT_IDX0                               PORT_CHANNEL_A
#define DRV_SCL_PIN_POSITION_IDX0                       PORTS_BIT_POS_2
#define DRV_SDA_PORT_IDX0                               PORT_CHANNEL_A
#define DRV_SDA_PIN_POSITION_IDX0                       PORTS_BIT_POS_3
#define DRV_I2C_BIT_BANG_IDX0                           false
#define DRV_I2C_STOP_IN_IDLE_IDX0                       false
#define DRV_I2C_SMBus_SPECIFICATION_IDX0			    false
#define DRV_I2C_BAUD_RATE_IDX0                    		400000
#define DRV_I2C_BRG_CLOCK_IDX0	                  		8000000
#define DRV_I2C_SLEW_RATE_CONTROL_IDX0      			false
#define DRV_I2C_MASTER_INT_SRC_IDX0               		INT_SOURCE_I2C_2_MASTER
#define DRV_I2C_SLAVE_INT_SRC_IDX0                		
#define DRV_I2C_ERR_MX_INT_SRC_IDX0               		INT_SOURCE_I2C_2_ERROR
#define DRV_I2C_INT_VECTOR_IDX0                         INT_VECTOR_I2C2
#define DRV_I2C_ISR_VECTOR_IDX0                         _I2C_2_VECTOR
#define DRV_I2C_INT_PRIORITY_IDX0                 		INT_PRIORITY_LEVEL1
#define DRV_I2C_INT_SUB_PRIORITY_IDX0             		INT_SUBPRIORITY_LEVEL0
#define DRV_I2C_POWER_STATE_IDX0                  		SYS_MODULE_POWER_RUN_FULL
#define DRV_I2C_INTERRUPT_MODE                    		true


/*** SPI Driver Configuration ***/
#define DRV_SPI_NUMBER_OF_MODULES		2
/*** Driver Compilation and static configuration options. ***/
/*** Select SPI compilation units.***/
#define DRV_SPI_POLLED 				0
#define DRV_SPI_ISR 				1
#define DRV_SPI_MASTER 				1
#define DRV_SPI_SLAVE 				0
#define DRV_SPI_RM 					0
#define DRV_SPI_EBM 				1
#define DRV_SPI_8BIT 				1
#define DRV_SPI_16BIT 				0
#define DRV_SPI_32BIT 				0
#define DRV_SPI_DMA 				0

/*** SPI Driver Static Allocation Options ***/
#define DRV_SPI_INSTANCES_NUMBER 		1
#define DRV_SPI_CLIENTS_NUMBER 			1
#define DRV_SPI_ELEMENTS_PER_QUEUE 		10
/* SPI Driver Instance 0 Configuration */
#define DRV_SPI_SPI_ID_IDX0 				SPI_ID_1
#define DRV_SPI_TASK_MODE_IDX0 				DRV_SPI_TASK_MODE_ISR
#define DRV_SPI_SPI_MODE_IDX0				DRV_SPI_MODE_MASTER
#define DRV_SPI_ALLOW_IDLE_RUN_IDX0			false
#define DRV_SPI_SPI_PROTOCOL_TYPE_IDX0 		DRV_SPI_PROTOCOL_TYPE_STANDARD
#define DRV_SPI_COMM_WIDTH_IDX0 			SPI_COMMUNICATION_WIDTH_8BITS
#define DRV_SPI_CLOCK_SOURCE_IDX0 		    SPI_BAUD_RATE_PBCLK_CLOCK
#define DRV_SPI_SPI_CLOCK_IDX0 				CLK_BUS_PERIPHERAL_1
#define DRV_SPI_BAUD_RATE_IDX0 				100000
#define DRV_SPI_BUFFER_TYPE_IDX0 			DRV_SPI_BUFFER_TYPE_ENHANCED
#define DRV_SPI_CLOCK_MODE_IDX0 			DRV_SPI_CLOCK_MODE_IDLE_LOW_EDGE_RISE
#define DRV_SPI_INPUT_PHASE_IDX0 			SPI_INPUT_SAMPLING_PHASE_IN_MIDDLE
#define DRV_SPI_TRANSMIT_DUMMY_BYTE_VALUE_IDX0      0xFF

#define DRV_SPI_TX_INT_SOURCE_IDX0 			INT_SOURCE_SPI_1_TRANSMIT
#define DRV_SPI_RX_INT_SOURCE_IDX0 			INT_SOURCE_SPI_1_RECEIVE
#define DRV_SPI_ERROR_INT_SOURCE_IDX0 		INT_SOURCE_SPI_1_ERROR
#define DRV_SPI_INT_VECTOR_IDX0				INT_VECTOR_SPI1
#define DRV_SPI_INT_PRIORITY_IDX0			INT_PRIORITY_LEVEL1
#define DRV_SPI_INT_SUB_PRIORITY_IDX0		INT_SUBPRIORITY_LEVEL0        
#define DRV_SPI_QUEUE_SIZE_IDX0 			10
#define DRV_SPI_RESERVED_JOB_IDX0 			1
/*** Timer Driver Configuration ***/
#define DRV_TMR_INTERRUPT_MODE             true
#define DRV_TMR_INSTANCES_NUMBER           1
#define DRV_TMR_CLIENTS_NUMBER             1

/*** Timer Driver 0 Configuration ***/
#define DRV_TMR_PERIPHERAL_ID_IDX0          TMR_ID_1
#define DRV_TMR_INTERRUPT_SOURCE_IDX0       INT_SOURCE_TIMER_1
#define DRV_TMR_INTERRUPT_VECTOR_IDX0       INT_VECTOR_T1
#define DRV_TMR_ISR_VECTOR_IDX0             _TIMER_1_VECTOR
#define DRV_TMR_INTERRUPT_PRIORITY_IDX0     INT_PRIORITY_LEVEL1
#define DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX0 INT_SUBPRIORITY_LEVEL0
#define DRV_TMR_CLOCK_SOURCE_IDX0           DRV_TMR_CLKSOURCE_INTERNAL
#define DRV_TMR_PRESCALE_IDX0               TMR_PRESCALE_VALUE_256
#define DRV_TMR_OPERATION_MODE_IDX0         DRV_TMR_OPERATION_MODE_16_BIT
#define DRV_TMR_ASYNC_WRITE_ENABLE_IDX0     false
#define DRV_TMR_POWER_STATE_IDX0            SYS_MODULE_POWER_RUN_FULL


 // *****************************************************************************
/* USART Driver Configuration Options
*/
#define DRV_USART_INTERRUPT_MODE                    true

#define DRV_USART_BYTE_MODEL_SUPPORT                false

#define DRV_USART_READ_WRITE_MODEL_SUPPORT          true

#define DRV_USART_BUFFER_QUEUE_SUPPORT              true

#define DRV_USART_CLIENTS_NUMBER                    2
#define DRV_USART_INSTANCES_NUMBER                  2

#define DRV_USART_PERIPHERAL_ID_IDX0                USART_ID_1
#define DRV_USART_OPER_MODE_IDX0                    DRV_USART_OPERATION_MODE_NORMAL
#define DRV_USART_OPER_MODE_DATA_IDX0               
#define DRV_USART_INIT_FLAG_WAKE_ON_START_IDX0      false
#define DRV_USART_INIT_FLAG_AUTO_BAUD_IDX0          false
#define DRV_USART_INIT_FLAG_STOP_IN_IDLE_IDX0       false
#define DRV_USART_INIT_FLAGS_IDX0                   0
#define DRV_USART_BRG_CLOCK_IDX0                    8000000
#define DRV_USART_BAUD_RATE_IDX0                    115200
#define DRV_USART_LINE_CNTRL_IDX0                   DRV_USART_LINE_CONTROL_8NONE1
#define DRV_USART_HANDSHAKE_MODE_IDX0               DRV_USART_HANDSHAKE_NONE
#define DRV_USART_LINES_ENABLE_IDX0                 USART_ENABLE_TX_RX_USED
#define DRV_USART_XMIT_INT_SRC_IDX0                 INT_SOURCE_USART_1_TRANSMIT
#define DRV_USART_RCV_INT_SRC_IDX0                  INT_SOURCE_USART_1_RECEIVE
#define DRV_USART_ERR_INT_SRC_IDX0                  INT_SOURCE_USART_1_ERROR
#define DRV_USART_INT_VECTOR_IDX0                   INT_VECTOR_UART1
#define DRV_USART_INT_PRIORITY_IDX0                 INT_PRIORITY_LEVEL1
#define DRV_USART_INT_SUB_PRIORITY_IDX0             INT_SUBPRIORITY_LEVEL0

#define DRV_USART_XMIT_QUEUE_SIZE_IDX0              10
#define DRV_USART_RCV_QUEUE_SIZE_IDX0               10


#define DRV_USART_POWER_STATE_IDX0                  SYS_MODULE_POWER_RUN_FULL

#define DRV_USART_PERIPHERAL_ID_IDX1                USART_ID_2
#define DRV_USART_OPER_MODE_IDX1                    DRV_USART_OPERATION_MODE_NORMAL
#define DRV_USART_OPER_MODE_DATA_IDX1               
#define DRV_USART_INIT_FLAG_WAKE_ON_START_IDX1      false
#define DRV_USART_INIT_FLAG_AUTO_BAUD_IDX1          false
#define DRV_USART_INIT_FLAG_STOP_IN_IDLE_IDX1       false
#define DRV_USART_INIT_FLAGS_IDX1                   0
#define DRV_USART_BRG_CLOCK_IDX1                    8000000
#define DRV_USART_BAUD_RATE_IDX1                    115200
#define DRV_USART_LINE_CNTRL_IDX1                   DRV_USART_LINE_CONTROL_8NONE1
#define DRV_USART_HANDSHAKE_MODE_IDX1               DRV_USART_HANDSHAKE_NONE
#define DRV_USART_LINES_ENABLE_IDX1                 USART_ENABLE_TX_RX_USED
#define DRV_USART_XMIT_INT_SRC_IDX1                 INT_SOURCE_USART_2_TRANSMIT
#define DRV_USART_RCV_INT_SRC_IDX1                  INT_SOURCE_USART_2_RECEIVE
#define DRV_USART_ERR_INT_SRC_IDX1                  INT_SOURCE_USART_2_ERROR
#define DRV_USART_INT_VECTOR_IDX1                   INT_VECTOR_UART2
#define DRV_USART_INT_PRIORITY_IDX1                 INT_PRIORITY_LEVEL1
#define DRV_USART_INT_SUB_PRIORITY_IDX1             INT_SUBPRIORITY_LEVEL0

#define DRV_USART_XMIT_QUEUE_SIZE_IDX1              10
#define DRV_USART_RCV_QUEUE_SIZE_IDX1               10


#define DRV_USART_POWER_STATE_IDX1                  SYS_MODULE_POWER_RUN_FULL

#define DRV_USART_QUEUE_DEPTH_COMBINED              40

// *****************************************************************************
// *****************************************************************************
// Section: Middleware & Other Library Configuration
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************
/*** Application Defined Pins ***/

/*** Functions for LedStatus pin ***/
#define LedStatusToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_9)
#define LedStatusOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_9)
#define LedStatusOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_9)
#define LedStatusStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_9)
#define LedStatusStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_9, Value)

/*** Functions for LedHeartbeat pin ***/
#define LedHeartbeatToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_13)
#define LedHeartbeatOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_13)
#define LedHeartbeatOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_13)
#define LedHeartbeatStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_13)
#define LedHeartbeatStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_13, Value)

/*** Functions for NRF_CE pin ***/
#define NRF_CEToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_15)
#define NRF_CEOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_15)
#define NRF_CEOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_15)
#define NRF_CEStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_15)
#define NRF_CEStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_15, Value)

/*** Functions for NRF_CS pin ***/
#define NRF_CSToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_0)
#define NRF_CSOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_0)
#define NRF_CSOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_0)
#define NRF_CSStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_0)
#define NRF_CSStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_0, Value)

/*** Functions for StatusRight pin ***/
#define StatusRightToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_0)
#define StatusRightOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_0)
#define StatusRightOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_0)
#define StatusRightStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_0)
#define StatusRightStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_0, Value)

/*** Functions for ControlRight pin ***/
#define ControlRightToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_2)
#define ControlRightOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_2)
#define ControlRightOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_2)
#define ControlRightStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_2)
#define ControlRightStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_2, Value)

/*** Functions for ControlLeft pin ***/
#define ControlLeftToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_9)
#define ControlLeftOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_9)
#define ControlLeftOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_9)
#define ControlLeftStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_9)
#define ControlLeftStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_9, Value)

/*** Functions for BT_Reset pin ***/
#define BT_ResetToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_4)
#define BT_ResetOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_4)
#define BT_ResetOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_4)
#define BT_ResetStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_4)
#define BT_ResetStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_4, Value)

/*** Functions for StatusLeft pin ***/
#define StatusLeftToggle() PLIB_PORTS_PinToggle(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_8)
#define StatusLeftOn() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_8)
#define StatusLeftOff() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_8)
#define StatusLeftStateGet() PLIB_PORTS_PinGetLatched(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_8)
#define StatusLeftStateSet(Value) PLIB_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_8, Value)

/*** Functions for BQ_PGood pin ***/
#define BQ_PGoodStateGet() PLIB_PORTS_PinGet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_6)


/*** Application Instance 0 Configuration ***/

/*** Application Instance 1 Configuration ***/

/*** Application Instance 2 Configuration ***/

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // _SYSTEM_CONFIG_H
/*******************************************************************************
 End of File
*/
