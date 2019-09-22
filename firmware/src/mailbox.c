#include "mailbox.h"


int8_t sendTo(uint8_t src, uint8_t dst, Packet *packet);

int8_t SendPacketToBle(uint8_t src, Packet *packet) {
    return sendTo(src, BLEOUT_MSG_ID, packet);
}

int8_t SendPacketToMainapp(uint8_t src, Packet *packet) {
    return sendTo(src, MAIN_MSG_ID, packet);
}

int8_t SendPacketToNrfApp(uint8_t src, Packet *packet) {
    return sendTo(src, NRF_MSG_ID, packet);
}

int8_t sendTo(uint8_t src, uint8_t dst, Packet *packet) {
    SYS_MSG_OBJECT message;
    message.nMessageTypeID = dst;
    message.nSource = src;
    message.nSizeData = sizeof(Packet);
    message.pData = (uintptr_t*)packet;
    SYS_MSG_RESULTS myRes = SYS_MSG_MessageSend(SYS_MSG_0, &message);
    if(myRes != SYS_MSG_SENT) {
        ERROR("Failed to send!! %d", myRes);
        return -1;
    } 
    return 0;
}
