#include "laserIfc.h"

SYS_MSG_OBJECT laserMsg = {
    nMessageID = LASER_MSG_ID
};

void LaserIfc_TurnOn(const uint8_t LASER){
   //laserMsg.nMessageID = LASER_MSG_ID;
   laserMsg.param0 = LASER;
}

void LaserIfc_TurnOff(const uint8_t LASER){
   laserMsg.param0 = ~LASER;
}
