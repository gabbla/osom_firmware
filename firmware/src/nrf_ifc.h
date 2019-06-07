#ifndef NRF_IFC_H_3P8AGRVE
#define NRF_IFC_H_3P8AGRVE

#include <peripheral/peripheral.h>
#include "memory_map.h"
#include "logger.h"

#define NRF_SPI_ID  SPI_ID_1

#define spi_active() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_0); asm("nop")
#define spi_idle()   PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_0); asm("nop")

#define nrf_set_ce() PLIB_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_15); asm("nop")
#define nrf_clr_ce() PLIB_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_15); asm("nop")

typedef union {
    struct {
        uint8_t tx_full : 1;
        uint8_t rx_pipe_number : 3;
        uint8_t max_retransmissions : 1;
        uint8_t tx_data_sent : 1;
        uint8_t rx_data_ready : 1;
        uint8_t : 1;
    };
    uint8_t status;
} NRF_Status;

/*
 * @brief Initialize the chip at default state
 * @return Chip STATE register
 */
NRF_Status NRF_Initialize();

/*
 * @brief Get the chip STATUS register
 * @return Chip STATUS register
 */
NRF_Status NRF_GetStatus();

/*
 * @brief Turn On or Off the chip
 * @param power chip power
 * @return Chip STATUS register
 */
NRF_Status NRF_PowerEnable(const bool power);

/*
 * @brief Clean the interrupts flags
 * @param int_mask Any ORed combination of RX_DR, TX_DS and MAX_RT
 * @return Chip STATUS register
 */
NRF_Status NRF_CleanInterrupts(const uint8_t int_mask);

/*
 * @brief Enable the interrupts reflection on IRQ pin
 * @param int_mask Any ORed combination of RX_DR,_TX_DS and MAX_RT
 * @return Chip STATUS register
 */
NRF_Status NRF_EnableInterrupts(const uint8_t int_mask);

/*
 * @brief Disable the interrupts reflection on IRQ pin
 * @param int_mask Any ORed combination of RX_DR,_TX_DS and MAX_RT
 * @return Chip STATUS register
 */
NRF_Status NRF_DisableInterrupts(const uint8_t int_mask);

/*
 * @brief Setup the address widthsm common for all data pipes
 * @param aw AW_x_BYTES where x = {3, 4, 5}. Cannot be ORed
 * @return Chip STATUS register
 */
NRF_Status NRF_SetAddressWidth(const uint8_t aw);

/*
 * @brief Set the pipe0 address. Write max AW bytes
 * @param addr vector filled with address, LSB first
 * @oaram len address length
 * @return Chip STATUS register
 */
NRF_Status NRF_SetPipe0Address(const uint8_t *addr, const size_t len);

/*
 * @brief Set the pipe 1-5 base address. Write max AW bytes.
 * @param addr vector filled with address, LSB first
 * @oaram len address length
 * @return Chip STATUS register
 */
NRF_Status NRF_SetPipeBaseAddress(const uint8_t *addr, const size_t len);

/*
 * @brief Set the pipe 1-5 LSB
 * @param pipe Index from 1 to 5
 * @param addr byte addr to set to pipe
 * @return Chip STATUS register
 */
NRF_Status NRF_SetPipeXAddress(const uint8_t pipe, const uint8_t addr);

NRF_Status NRF_StartListening();


/*
 * @brief Send the command FLUSH_TX to the chip
 * @return Chip STATUS register
 */
NRF_Status NRF_FlushTx();

/*
 * @brief Send the command FLUSH_RX to the chip
 * @return Chip STATUS register
 */
NRF_Status NRF_FlushRx();

#endif /* end of include guard: NRF_IFC_H_3P8AGRVE */
