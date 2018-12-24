#include "mailbox.h"


int8_t SendPacketToBle(uint8_t src, Packet *packet) {
    SYS_MSG_OBJECT message;
    message.nMessageTypeID = BLEOUT_MSG_ID;
    message.nSource = src;
    message.nSizeData = sizeof(Packet);
    message.pData = (uintptr_t*)packet;
    SYS_MSG_RESULTS myRes = SYS_MSG_MessageSend(BLEOUT_MAILBOX, &message);
    if(myRes != SYS_MSG_SENT) {
        ERROR("Failed to send!! %d", myRes);
        return -1;
    } 
    return 0;
}
