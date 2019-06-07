#include "nrf_ifc.h"

NRF_Status nrf_write_register_size(const uint8_t reg, const uint8_t *data, const size_t len) {
    //DEBUG("%s() reg: 0x%02X value: 0x%02X", __func__, reg, value);
    PLIB_SPI_BufferClear(NRF_SPI_ID);
    spi_active();
    PLIB_SPI_BufferWrite(NRF_SPI_ID, W_REGISTER | (REGISTER_MASK & reg));
    size_t i;
    for(i = 0; i < len; ++i)
        PLIB_SPI_BufferWrite(NRF_SPI_ID, data[i]);
    while (!PLIB_SPI_FIFOShiftRegisterIsEmpty(NRF_SPI_ID))
        ; // Wait until the shift register is empty
    spi_idle();
    return (NRF_Status)PLIB_SPI_BufferRead(NRF_SPI_ID);
}

NRF_Status nrf_write_register(const uint8_t reg, const uint8_t value) {
    return nrf_write_register_size(reg, &value, 1);
}

NRF_Status nrf_read_register(const uint8_t reg, uint8_t *value) {
    //DEBUG("%s() reg: 0x%02X", __func__, reg);
    NRF_Status status;
    PLIB_SPI_BufferClear(NRF_SPI_ID);
    spi_active();
    PLIB_SPI_BufferWrite(NRF_SPI_ID, R_REGISTER | (REGISTER_MASK & reg));
    PLIB_SPI_BufferWrite(NRF_SPI_ID, 0); // Just generate the clock
    while (!PLIB_SPI_FIFOShiftRegisterIsEmpty(NRF_SPI_ID))
        ; // Wait untill the shift register is empty
    spi_idle();
    status.status = PLIB_SPI_BufferRead(NRF_SPI_ID); // status
    *value = PLIB_SPI_BufferRead(NRF_SPI_ID);
    //DEBUG("%s() reg: 0x%02X value: 0x%02X", __func__, reg, value);
    return status;
}

NRF_Status nrf_send_command(const uint8_t cmd) {
    PLIB_SPI_BufferClear(NRF_SPI_ID);
    spi_active();
    PLIB_SPI_BufferWrite(NRF_SPI_ID, cmd);
    while (!PLIB_SPI_FIFOShiftRegisterIsEmpty(NRF_SPI_ID))
        ; // Wait until the shift register is empty
    spi_idle();
    return (NRF_Status)PLIB_SPI_BufferRead(NRF_SPI_ID);
}

NRF_Status NRF_Initialize() {
    // Power down the module
    NRF_PowerEnable(false);
    // Enable irq reflection (default)
    NRF_EnableInterrupts(RX_DR | TX_DS | MAX_RT);
    // Clear interrupts
    NRF_CleanInterrupts(RX_DR | TX_DS | MAX_RT);
    // Flush pipes
    NRF_FlushRx();
    NRF_FlushTx();
    return NRF_GetStatus();
}

NRF_Status NRF_GetStatus() {
    return nrf_send_command(NOP);
}

NRF_Status NRF_PowerEnable(const bool power) {
    uint8_t config;
    nrf_read_register(CONFIG, &config);
    if(power)
        config |= (1 << PWR_UP);
    else
        config &= ~(1 << PWR_UP);
    return nrf_write_register(CONFIG, config);
}

NRF_Status NRF_CleanInterrupts(const uint8_t int_mask) {
    uint8_t status;
    nrf_read_register(STATUS, &status);
    if(int_mask & RX_DR)
        status |= (1 << RX_DR);
    if(int_mask & TX_DS)
        status |= (1 << TX_DS);
    if(int_mask & MAX_RT)
        status |= (1<< MAX_RT);
    return nrf_write_register(STATUS, status);
}

NRF_Status NRF_EnableInterrupts(const uint8_t int_mask) {
    uint8_t config;
    nrf_read_register(CONFIG, &config);
    if(int_mask & RX_DR)
        config &= ~(1 << MASK_RX_DR);
    if(int_mask & MASK_TX_DS)
        config &= ~(1 << TX_DS);
    if(int_mask & MASK_MAX_RT)
        config &= ~(1<< MAX_RT);
    return nrf_write_register(CONFIG, config);
}

NRF_Status NRF_DisableInterrupts(const uint8_t int_mask) {
    uint8_t config;
    nrf_read_register(CONFIG, &config);
    if(int_mask & RX_DR)
        config |= (1 << MASK_RX_DR);
    if(int_mask & MASK_TX_DS)
        config |= (1 << TX_DS);
    if(int_mask & MASK_MAX_RT)
        config |= (1<< MAX_RT);
    return nrf_write_register(CONFIG, config);
}

NRF_Status NRF_SetAddressWidth(const uint8_t aw) {
    // No need to read SETUP_AW
    return nrf_write_register(SETUP_AW, aw);
}

NRF_Status NRF_SetPipe0Address(const uint8_t *addr, const size_t len) {
    // TODO check on AW?
    return nrf_write_register_size(RX_ADDR_P0, addr, len);
}

NRF_Status NRF_SetPipeBaseAddress(const uint8_t *addr, const size_t len) {
    // TODO check on AW?
    return nrf_write_register_size(RX_ADDR_P1, addr, len);
}

NRF_Status NRF_SetPipeXAddress(const uint8_t pipe, const uint8_t addr) {
    return nrf_write_register(RX_ADDR_P0 + pipe, addr);
}

NRF_Status NRF_StartListening() {
    nrf_set_ce();
    return nrf_write_register(CONFIG, (1 << PWR_UP) | (1 << PRIM_RX));

}



NRF_Status NRF_FlushTx() {
    return nrf_send_command(FLUSH_TX);
}

NRF_Status NRF_FlushRx() {
    return nrf_send_command(FLUSH_RX);
}
