#include "powerapp.h"

POWERAPP_DATA powerappData;

/*
 * Return the first free request, NULL otherwise
 */
BQ_Request *getFreeRequest(BQ_Request *reqs, const size_t size) {
    int i;
    for (i = 0; i < size; ++i)
        if(reqs[i].status == REQ_FREE)
            return &reqs[i];
    return NULL;
}

BQ_Request *getNextRequest(BQ_Request *reqs, const size_t size) {
    int i;
    for (i = 0; i < size; ++i)
        if (reqs[i].status == REQ_PENDING) return &reqs[i];
    return NULL;
}

bool BQ27441_GetData(const BQ27441_Command cmd, BQ27441_CALLBACK cb) {
    BQ_Request *req = getFreeRequest(powerappData.requests, MAX_BQ_REQUESTS);
    if (!req) return false;
    req->status = REQ_PENDING;
    uint8_t tmp[] = {(uint8_t)cmd, (uint8_t)cmd + 1};
    memcpy(req->bqCommand, tmp, 2);
    req->bqCallback = cb;
    req->bqCmdId = cmd;
    return true;
}

/*******************************************************************************
  Function:
    void POWERAPP_Initialize ( void )

  Remarks:
    See prototype in powerapp.h.
 */

void POWERAPP_Initialize(void) {
    powerappData.state = POWERAPP_STATE_INIT;
    memset(powerappData.requests, 0, MAX_BQ_REQUESTS * sizeof(BQ_Request));
}

/******************************************************************************
  Function:
    void POWERAPP_Tasks ( void )

  Remarks:
    See prototype in powerapp.h.
 */

uint8_t voltage[] = {4, 5, 0};
uint8_t received[] = {0, 0, 0};

void POWERAPP_Tasks(void) {
    /* Check the application's current state. */
    switch (powerappData.state) {
        /* Application's initial state. */
        case POWERAPP_STATE_INIT: {
            bool appInitialized = true;
            powerappData.gauge =
                DRV_I2C_Open(DRV_I2C_INDEX_0, DRV_IO_INTENT_READWRITE);
            if (powerappData.gauge == DRV_HANDLE_INVALID) {
                ERROR("Gauge I2C handler is invalid");
                appInitialized = false;
            }

            asm("nop");
            if (appInitialized) {
                powerappData.state = POWERAPP_STATE_IDLE;
            }
            break;
        }

        case POWERAPP_STATE_SERVICE_TASKS: {
            break;
        }

        case POWERAPP_STATE_IDLE: {
            powerappData.currentRequest =
                getNextRequest(powerappData.requests, MAX_BQ_REQUESTS);
            if(powerappData.currentRequest) {
                DEBUG("Request to serve found. Command: 0x%X", powerappData.currentRequest->bqCmdId);
                powerappData.state = POWERAPP_STATE_GET_DATA;
            }
            break;
        }

        case POWERAPP_STATE_GET_DATA: {
            powerappData.currentRequest->hBuff = DRV_I2C_TransmitThenReceive(
                powerappData.gauge, 0xAA, powerappData.currentRequest->bqCommand, 2,
                powerappData.currentRequest->bqReply, 2, NULL);
            if (!powerappData.currentRequest->hBuff) {
                powerappData.state = POWERAPP_STATE_ERROR;
                break;
            }
            powerappData.state = POWERAPP_STATE_WAIT;
            break;
        }

        case POWERAPP_STATE_WAIT: {
            DRV_I2C_BUFFER_EVENT status = DRV_I2C_TransferStatusGet(
                powerappData.gauge, powerappData.currentRequest->hBuff);
            if (status == DRV_I2C_BUFFER_EVENT_COMPLETE) {
                powerappData.operationInProgress = false;
                powerappData.currentRequest->bqCallback(powerappData.currentRequest->bqCmdId,
                                        powerappData.currentRequest->bqReply, 2);
                powerappData.currentRequest->status = REQ_FREE;
                powerappData.state = POWERAPP_STATE_IDLE;
            } else if (status == DRV_I2C_BUFFER_EVENT_ERROR) {
                powerappData.state = POWERAPP_STATE_ERROR;
            }

            break;
        }

        case POWERAPP_STATE_ERROR: {
            powerappData.currentRequest->bqCallback(powerappData.currentRequest->bqCmdId, NULL, 0);
            powerappData.currentRequest->status = REQ_FREE;
            powerappData.operationInProgress = false;
            break;
        }

        /* The default state should never be executed. */
        default: { break; }
    }
}
