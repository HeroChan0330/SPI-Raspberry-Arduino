#include "ArduinoModule.h"

void connectInit(){
    spiInit();
}
bool getSensor(uint16_t*buffer){
    int cnt=0;
    while(spiTxRx(0x01<<5)==0){
        cnt++;
        if(cnt>MAX_TRY_CNT) return false;
    }
    uint8_t*ptr=(uint8_t*)buffer;
    for (int i = 0; i < 10; i++){
        ptr[i] = spiTxRx(0);
    }
    return true;
}

bool setLaserState(uint8_t state){
    int cnt=0;
    while(spiTxRx(0x02<<5|state)==0){
        cnt++;
        if(cnt>MAX_TRY_CNT) return false;
    }
    return true;
}