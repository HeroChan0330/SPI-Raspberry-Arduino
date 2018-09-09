#ifndef _ARDUINO_MODULE_H_
#define _ARDUINO_MODULE_H_

#include <stdint.h>
#include "SPI.h"

#define MAX_TRY_CNT 10
void connectInit();
bool getSensor(uint16_t*buffer);//buffer should len 5
bool setLaserState(uint8_t state);

#endif