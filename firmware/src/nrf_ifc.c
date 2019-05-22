#include "nrf_ifc.h"


NRF_Status nrf_write_register(const uint8_t reg, const uint8_t value) {
    //DEBUG("%s() reg: 0x%02X value: 0x%02X", __func__, reg, value);
    PLIB_SPI_BufferClear(NRF_SPI_ID);
    spi_active();
    PLIB_SPI_BufferWrite(NRF_SPI_ID, W_REGISTER | (REGISTER_MASK & reg));
    PLIB_SPI_BufferWrite(NRF_SPI_ID, value);
    while (!PLIB_SPI_FIFOShiftRegisterIsEmpty(NRF_SPI_ID))
        ; // Wait until the shift register is empty
    spi_idle();
    return (NRF_Status)PLIB_SPI_BufferRead(NRF_SPI_ID);
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
    if(status & RX_DR)
        status |= (1 << RX_DR);
    if(status & TX_DS)
        status |= (1 << TX_DS);
    if(status & MAX_RT)
        status |= (1<< MAX_RT);
    return nrf_write_register(STATUS, status);
}

NRF_Status NRF_FlushTx() {
    return nrf_send_command(FLUSH_TX);
}

NRF_Status NRF_FlushRx() {
    return nrf_send_command(FLUSH_RX);
}
