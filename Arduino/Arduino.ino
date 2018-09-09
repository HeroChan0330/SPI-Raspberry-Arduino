/*
 * Author:HeroChan
 * Date:18/7/27
 * Function:Gather analog values of the photoresistances and send back to raspberry through SPI connection
 * Usage:
 * 1.raspberry sends a command 0x01<<5 to Arduino to request the analog values, then arduino send a byte array(len=10) back to the raspberry
 * 2.raspberry sends a command 0x02<<5|switchs, switchs means the laser you want to open,it obeys the following format.
 *   switchs has 5 bits, each bit control the corresponding laser.
 */

#include <SPI.h>
//in the first element the buffer(len 10) we receive, represent the command the raspberry send to arduino
//the first 3 bits indicate the command type, last 5 bits indicate the value attached to the command
#define COMMAND_GET 0x01
#define COMMAND_SET_LASER 0x02

volatile int state=0;//状态机 0为空闲 1为正在读取Init指令 2为Get指令 3为设置激光状态指令 4为Init调用时查看是否完成采集数据
volatile int stateCnt=0;//状态机中一共读取多少次
volatile uint16_t analogData[5]={999,464,887,3222,2333};
volatile byte*toSend=(byte*)analogData;

enum SlaveState{
  SLAVE_STATE_IDLE,
  SLAVE_STATE_GET_ANALOG,
  SLAVE_STATE_SET_LASER
};


byte laserPins[5]={2,3,4,5,6};
byte sensorPins[5]={0,1,2,3,4};

                    
void setup (void){
  pinInit();
  Serial.begin(9600);
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);

  // 开启中断
  SPI.attachInterrupt();
}


// SPI Interrupt
//a state machine
ISR (SPI_STC_vect){
    byte recv=SPDR;
    //Serial.print(recv);
    byte comm=recv>>5;
    switch(state){
      case SLAVE_STATE_IDLE://空闲状态
        switch(comm){
          case COMMAND_GET:
            state=SLAVE_STATE_GET_ANALOG;
            SPDR=1;
            return;
            //Serial.println("request analog");
          break;
          case COMMAND_SET_LASER:
            SPDR=1;
            setLaserState(recv);
            state=SLAVE_STATE_SET_LASER;
            return;
          break;
        }
      break;
      case SLAVE_STATE_GET_ANALOG:
        SPDR=toSend[stateCnt++];
        if(stateCnt==10){ //发送完毕进入空闲
          state=SLAVE_STATE_IDLE;
          stateCnt=0;
        }
        return;
      break;
      case SLAVE_STATE_SET_LASER:
         SPDR=1;
         state=SLAVE_STATE_IDLE;
         return;
      break;
    }
    //while(!(SPSR & (1<<SPIF)));
}

void loop (void){
  getAnalogValues();
  delay(10);
}

void pinInit(){
  for(int i=0;i<5;i++){
    pinMode(laserPins[i],OUTPUT);
    digitalWrite(laserPins[i],HIGH);//open the laser when init defaultly
  }
}

void getAnalogValues(){
  for(int i=0;i<5;i++){
    analogData[i]=analogRead(sensorPins[i]);
//    Serial.print(analogData[i]);
//    Serial.print(' ');
  }
//  Serial.print('\n');
}

void setLaserState(byte state){
//  Serial.print("Laser state: ");
//  for(byte i=16;i>0;i>>=1){
//    Serial.print(state&i?"On ":"Off ");
//  }
//  Serial.print("\n");

  byte mask=16;
  for(int i=0;i<5;i++){
    if(state&mask){
      digitalWrite(laserPins[i],HIGH);
    }else{
      digitalWrite(laserPins[i],LOW);
    }
    mask>>=1;
  }
}


