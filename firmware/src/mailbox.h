#ifndef MAILBOX_H_VY2CRLWE
#define MAILBOX_H_VY2CRLWE

#include "somparser.h" // For packet
#include "logger.h" // log utils
#include "system_config/pic32mx274f256d/system_definitions.h"
// Useful stuff about mailboxes
// Sources
#define MSG_SRC_BLE             (0)
#define MSG_SRC_MAIN            (1)
#define MSG_SRC_NRF             (2)

static const char *srcName[3]= {
    "MSG_SRC_BLE",
    "MSG_SRC_MAIN",
    "MSG_SRC_NRF"
};

#define MAILBOX_SRC_TO_STR(x) (srcName[x])

// BLE Outgoing
#define BLEOUT_MAILBOX          (SYS_MSG_0)
#define BLEOUT_MSG_PRIORITY     (0)
#define BLEOUT_MSG_ID           (0)

// MAIN
#define MAIN_MAILBOX           (SYS_MSG_0)
#define MAIN_MSG_PRIORITY      (0)
#define MAIN_MSG_ID            (1)

// NRF
#define NRF_MAILBOX           (SYS_MSG_0)
#define NRF_MSG_PRIORITY      (0)
#define NRF_MSG_ID            (2)

// Utils

int8_t SendPacketToBle(uint8_t src, Packet *packet);
int8_t SendPacketToMainapp(uint8_t src, Packet *packet);
#endif /* end of include guard: MAILBOX_H_VY2CRLWE */
