#ifndef MAILBOX_H_VY2CRLWE
#define MAILBOX_H_VY2CRLWE

#include "somparser.h" // For packet
#include "logger.h" // log utils
#include "system_config/pic32mx274f256d/system_definitions.h"
// Useful stuff about mailboxes
// Sources
#define MSG_SRC_BLE             (0)
#define MSG_SRC_MAIN            (1)
#define MSG_SRC_COM             (2)

// BLE Outgoing
#define BLEOUT_MAILBOX          (SYS_MSG_0)
#define BLEOUT_MSG_PRIORITY     (0)
#define BLEOUT_MSG_ID           (0)

// MAIN
#define MAIN_MAILBOX           (SYS_MSG_0)
#define MAIN_MSG_PRIORITY      (0)
#define MAIN_MSG_ID            (1)


// Utils

int8_t SendPacketToBle(uint8_t src, Packet *packet);

#endif /* end of include guard: MAILBOX_H_VY2CRLWE */
