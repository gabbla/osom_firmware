#include "nrf_ifc.h"

static const uint8_t child_pipe[] = {RX_ADDR_P0, RX_ADDR_P1, RX_ADDR_P2,
                                     RX_ADDR_P3, RX_ADDR_P4, RX_ADDR_P5};

static const uint8_t child_pipe_enable[] = {ERX_P0, ERX_P1, ERX_P2,
                                            ERX_P3, ERX_P4, ERX_P5};

static const uint8_t child_dynpd[] = {DPL_P0, DPL_P1, DPL_P2,
                                      DPL_P3, DPL_P4, DPL_P5};

static const uint8_t child_payload_size[] = {RX_PW_P0, RX_PW_P1, RX_PW_P2,
                                             RX_PW_P3, RX_PW_P4, RX_PW_P5};

static const uint8_t child_esb[] = {ENAA_P0, ENAA_P1, ENAA_P2,
                                    ENAA_P3, ENAA_P4, ENAA_P5};

static const uint8_t reg[] = {
    CONFIG,     EN_AA,      EN_RXADDR,  SETUP_AW,   SETUP_RETR, RF_CH,
    RF_SETUP,   STATUS,     OBSERVE_TX, CD,         RX_ADDR_P0, RX_ADDR_P1,
    RX_ADDR_P2, RX_ADDR_P3, RX_ADDR_P4, RX_ADDR_P5, TX_ADDR,    RX_PW_P0,
    RX_PW_P1,   RX_PW_P2,   RX_PW_P3,   RX_PW_P4,   RX_PW_P5,   FIFO_STATUS,
    DYNPD,      FEATURE};

static uint64_t rx_addr_p0;

static _NRF_Internal internal = {
    .flags.dyn_payload = 0,
    .flags.p_variant = 0,

    .address_width = 5,
    .payload_size = 32,
};

void __delay_us(uint16_t microseconds) {
    uint32_t time;
    time = _CP0_GET_COUNT(); // Read Core Timer
    time += (SYS_CLK_FREQ / 2 / 1000000) * microseconds; // calc the Stop Time
    while ((int32_t)(time - _CP0_GET_COUNT()) > 0){};
}

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
    // Set CRC len to 2bytes
    NRF_SetCrcLen(NRF_CRC_2_BYTE);
    // Set 15 retries every (5 + 1) * 250 uS
    NRF_SetRetries(15, 5);
    // Set 1Mbps
    NRF_SetBaudrate(NRF_1MBPS);
    // TODO maybe I can get rid of this flag
    internal.flags.p_variant = 1;
    // Disable all the features
    NRF_DisableFeatures(EN_DPL | EN_ACK_PAY | EN_DYN_ACK);
    // Disable all the dynamic payloads
    nrf_write_register(DYNPD, 0);
    internal.flags.dyn_payload = 0;
    // Clear irqs
    NRF_CleanInterrupts(RX_DR | TX_DS | MAX_RT);
    NRF_SetChannel(76);
    // Flush the pipes
    NRF_FlushRx();
    NRF_FlushTx();
    // Power the module
    NRF_PowerEnable(true);
    // Device as transmitter
    NRF_SetMode(NRF_PTX);
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

NRF_Status NRF_SetMode(const NRF_Mode mode) {
    uint8_t config;
    nrf_read_register(CONFIG, &config);
    if(mode == NRF_PRX) // as receiver
        config |= _BV(PRIM_RX);
    else
        config &= ~_BV(PRIM_RX);
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

void addressToVect(const uint64_t addr, uint8_t *v){
    v[0] = ((uint8_t)(addr >> 0));
    v[1] = ((uint8_t)(addr >> 8));
    v[2] = ((uint8_t)(addr >> 16));
    v[3] = ((uint8_t)(addr >> 24));
    v[4] = ((uint8_t)(addr >> 32));
}

NRF_Status NRF_SetPipeAddress(const uint8_t pipe, const uint64_t address) {
    uint8_t vAddr[5];
    addressToVect(address, vAddr);
    NRF_Status status;
    if(pipe == 0)
        rx_addr_p0 = address;
    if(pipe <= 2)
        status = nrf_write_register_size(child_pipe[pipe], vAddr, 5);
    else
        status = nrf_write_register_size(child_pipe[pipe], vAddr, 1);
    return status;
}

NRF_Status NRF_SetTxAddress(const uint64_t address) {
    uint8_t addr[5];
    addressToVect(address, addr);
    return nrf_write_register_size(TX_ADDR, addr, 5);
}

NRF_Status NRF_OpenWritingPipe(const uint64_t addr) {
    uint8_t vAddr[5];
    addressToVect(addr, vAddr);
    nrf_write_register_size(RX_ADDR_P0, vAddr, internal.address_width);
    nrf_write_register_size(TX_ADDR, vAddr, internal.address_width);

    return nrf_write_register(RX_PW_P0, internal.payload_size);
}

NRF_Status NRF_CloseReadingPipe(const uint8_t pipe) {
    uint8_t en_rxaddr;
    nrf_read_register(EN_RXADDR, &en_rxaddr);
    return nrf_write_register(EN_RXADDR,
                              en_rxaddr & ~_BV(child_pipe_enable[pipe]));
}

NRF_Status NRF_OpenReadingPipe(const uint8_t pipe, const uint64_t addr) {
    // Cache pipe0 address
    if (pipe == 0) internal.pipe0_address = addr;

    if (pipe <= 6) {
        if (pipe < 2) {
            uint8_t vAddr[5];
            addressToVect(addr, vAddr);
            nrf_write_register_size(child_pipe[pipe], vAddr,
                                    internal.address_width);
        } else {
            nrf_write_register(child_pipe[pipe], (uint8_t)(addr >> 32));
        }
        nrf_write_register(child_payload_size[pipe], internal.payload_size);

        uint8_t en_rxaddr;
        nrf_read_register(EN_RXADDR, &en_rxaddr);
        return nrf_write_register(EN_RXADDR,
                                  en_rxaddr | _BV(child_pipe_enable[pipe]));
    }
    return NRF_GetStatus();
}

NRF_Status NRF_SetChannel(const uint8_t channel) {
    return nrf_write_register(RF_CH, (channel & 0x7F));
}

NRF_Status NRF_SetBaudrate(const speed_t baudrate) {
    uint8_t rf_setup;
    nrf_read_register(RF_SETUP, &rf_setup);
    // XXX remove me
    return nrf_write_register(RF_SETUP, 0x07);
    // Go for 0bXX0X0XXX
    rf_setup &= (~((1 << RF_DR_LOW) | (1 << RF_DR_HIGH)));
    switch(baudrate) {
        case NRF_1MBPS:
            break;
        case NRF_2MBPS:
            rf_setup |= (1 << RF_DR_HIGH);
            break;
        case NRF_250KBPS:
            rf_setup |= (1 << RF_DR_LOW);
            break;
    }
    return nrf_write_register(RF_SETUP, rf_setup);
}

NRF_Status NRF_SetCrcLen(const NRF_CRC_LEN len) {
    uint8_t config;
    nrf_read_register(CONFIG, &config);
    if(len == NRF_CRC_2_BYTE)
        config |= (1 << CRCO);
    else // 1 byte
        config &= (~(1 << CRCO));
    return nrf_write_register(CONFIG, config);
}

NRF_Status NRF_SetRetries(const uint8_t retries, const uint8_t delay) {
    uint8_t setup_retr = ((delay << ARD) | (retries & 0x0F));
    return nrf_write_register(SETUP_RETR, setup_retr);
}

NRF_Status NRF_SetPALevel(const NRF_PA level) {
    uint8_t rf_setup, l;
    nrf_read_register(RF_SETUP, &rf_setup);
    // Set the 3 LSB to 0
    rf_setup &= 0xF8;
    if(level > NRF_PA_MAX)
        l = (NRF_PA_MAX << 1);
    else
        l = (level << 1);
    return nrf_write_register(RF_SETUP, rf_setup | l);
}

NRF_Status NRF_EnableFeatures(const uint8_t features) {
    uint8_t feature;
    nrf_read_register(FEATURE, &feature);
    if(features & EN_DPL)
        feature |= _BV(EN_DPL);
    if(features & EN_ACK_PAY)
        feature |= _BV(EN_ACK_PAY);
    if(features & EN_DYN_ACK)
        feature |= _BV(EN_DYN_ACK);
    return nrf_write_register(FEATURE, feature);
}

NRF_Status NRF_DisableFeatures(const uint8_t features) {
    uint8_t feature;
    nrf_read_register(FEATURE, &feature);
    if(features & EN_DPL)
        feature &= ~_BV(EN_DPL);
    if(features & EN_ACK_PAY)
        feature &= ~_BV(EN_ACK_PAY);
    if(features & EN_DYN_ACK)
        feature &= ~_BV(EN_DYN_ACK);
    return nrf_write_register(FEATURE, feature);
}

NRF_Status NRF_EnableDynPayload(const uint8_t pipe) {
    // Each operation that enable DYNPD needs bit EN_DPL and ENAA_Px set
    NRF_EnableFeatures(EN_DPL);
    uint8_t dynpd, en_aa;
    nrf_read_register(DYNPD, &dynpd);
    NRF_EnableEnanchedShockBurst(pipe);
    // FIXME this will enable only one dyn payload at once
    return nrf_write_register(DYNPD, _BV(child_dynpd[pipe]));
}

NRF_Status NRF_DisableDynPayload(const uint8_t pipe) {
    uint8_t dynpd;
    nrf_read_register(DYNPD, &dynpd);
    return nrf_write_register(DYNPD, dynpd & ~_BV(child_dynpd[pipe]));
}

NRF_Status NRF_EnableRxPipe(const uint8_t pipe) {
    uint8_t en_rxaddr;
    nrf_read_register(EN_RXADDR, &en_rxaddr);
    en_rxaddr |= _BV(child_pipe_enable[pipe]);
    return nrf_write_register(EN_RXADDR, en_rxaddr);
}

NRF_Status NRF_EnableEnanchedShockBurst(const uint8_t pipe) {
    uint8_t en_aa;
    nrf_read_register(EN_AA, &en_aa);
    en_aa |= _BV(child_esb[pipe]);
    return nrf_write_register(EN_AA, en_aa);
}

NRF_Status write_payload(const void *data, const size_t size) {
    NRF_Status status;
    size_t len = size;
    uint8_t *current = (uint8_t*)data;
    PLIB_SPI_BufferClear(NRF_SPI_ID);
    spi_active();

    // send the W_TX_PAYLOAD
    PLIB_SPI_BufferWrite(NRF_SPI_ID, W_TX_PAYLOAD);
    while (!PLIB_SPI_FIFOShiftRegisterIsEmpty(NRF_SPI_ID))
        ; // Wait until the shift register is empty
    status = (NRF_Status)PLIB_SPI_BufferRead(NRF_SPI_ID);
    while(len--){
        // TODO verifiy enenached SPI
        PLIB_SPI_BufferWrite(NRF_SPI_ID, *current);
        current++;
    }
    spi_idle();
    return status;
}

NRF_Status NRF_StartListening() {
    NRF_SetMode(NRF_PRX);
    NRF_CleanInterrupts(RX_DR | TX_DS | MAX_RT);
    if(internal.pipe0_address) {
        uint8_t vAddr[5];
        addressToVect(internal.pipe0_address, vAddr);
        nrf_write_register_size(RX_ADDR_P0, vAddr, internal.address_width);
    }else {
        NRF_CloseReadingPipe(0);
    }

    uint8_t feature;
    NRF_Status s = nrf_read_register(FEATURE, &feature);
    if(feature & _BV(EN_ACK_PAY))
        s = NRF_FlushTx();
    return s;
}

NRF_Status NRF_GetPayloadSize(const uint8_t pipe, uint8_t *size) {
    return nrf_read_register(child_payload_size[pipe], size);
}

NRF_Status NRF_Write(const uint8_t pipe, const void *data, const size_t size) {
    uint8_t config;
    NRF_Status status;
    nrf_read_register(CONFIG, &config);
    config = ((config | _BV(PWR_UP)) & ~_BV(PRIM_RX));
    // We need to wait 150uS to be sure the tx is powered up
    __delay_us(150);
    status = write_payload(data, size);

    // Transmit
    NRF_CEOn();
    __delay_us(15);
    NRF_CEOff();
    return status;
}

bool NRF_Available(uint8_t *pipe) {
    uint8_t fifo_status;
    nrf_read_register(FIFO_STATUS, &fifo_status);
    if (fifo_status & _BV(RX_EMPTY)) {
        if (pipe) {
            NRF_Status status = NRF_GetStatus();
            *pipe = status.rx_pipe_number;
        }
        return true;
    }
    return false;
}

NRF_Status NRF_DumpRegisters() {
    size_t len, i;
    uint8_t dummy;
    NRF_Status status;
    for(i = 0, len = sizeof(reg); i < len; ++i)
        status = nrf_read_register(reg[i], &dummy);
    return status;
}

NRF_Status NRF_ReadPayload(void *buf, const size_t len) {
    size_t total = len;
    uint8_t *current = (uint8_t*)buf;
    NRF_Status status = nrf_send_command(R_RX_PAYLOAD);
    while(total--)
        *current++ = nrf_send_command(NOP).status;
    return status;
}

NRF_Status NRF_FlushTx() {
    return nrf_send_command(FLUSH_TX);
}

NRF_Status NRF_FlushRx() {
    return nrf_send_command(FLUSH_RX);
}
