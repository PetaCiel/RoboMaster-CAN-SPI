#include "RoboMaster.h"

//CanNode Struct
CanNode nodeRx={0x000,{0}};
CanNode nodeTx={0x200,{0}};

//CanBus Member Function
CanBus::CanBus(SPIClass *CB_SPI, INT8U CB_CS)
  : RM_CAN(CB_SPI,CB_CS){
}
void CanBus::init(){
  if (RM_CAN.begin(MCP_ANY, CAN_1000KBPS, MCP_8MHZ) == CAN_OK)
  {
    Serial.println("RM_CAN: Init OK!");
    RM_CAN.setMode(MCP_NORMAL);
  }
  else
  {
    Serial.println("RM_CAN: Init Fail!");
  }
  Pre_millis = millis();
}
void CanBus::update(){
  if(RM_CAN.checkReceive() == CAN_MSGAVAIL){
    RM_CAN.readMsgBuf(&nodeRx.Id, &len, nodeRx.Buf);
  }
  RxId=nodeRx.Id-0x200;
  for(int i=0;i<8;i++){
    SubRxBuf[(RxId)-1][i]=nodeRx.Buf[i];
  }
}
void CanBus::set(){
  if (millis() - Pre_millis > 20) { // Period: 20ms
    RM_CAN.sendMsgBuf(nodeTx.Id, 0, 8, nodeTx.Buf);
    Pre_millis = millis();
  }
}





//RoboMaster Member Function
RoboMasterC620::RoboMasterC620(CanBus* bus, int id){
  MYBUS=bus;
  Id=id;
}

void RoboMasterC620::update(){
  for(int i=0;i<8;i++){
    rxBuf[i]=MYBUS->SubRxBuf[Id-1][i];
  }
}


int RoboMasterC620::getAngle(){
  angle = rxBuf[0] << 8 | rxBuf[1];
  return(angle);
}
int RoboMasterC620::getRPM(){
  rpm = rxBuf[2] << 8 | rxBuf[3];
  return(rpm);
}
int RoboMasterC620::getAmp(){
  amp = rxBuf[4] << 8 | rxBuf[5];
  return(amp);
}
int RoboMasterC620::getTemp(){
  temp = rxBuf[6];
  return(temp);
}
int16_t RoboMasterC620::floatmapping(double x, double in_min, double in_max, int16_t out_min, int16_t out_max){
  return (x - in_min) * ((double)(out_max - out_min)) / (in_max - in_min) + out_min;
}
void RoboMasterC620::setCurrent(double current){
  int16_t output_current = floatmapping(current,0, 20, 0, 16384);
  switch(Id){
    case 1: nodeTx.Buf[0] = (output_current >> 8) & 0xFF; nodeTx.Buf[1] = output_current & 0xFF; break;
    case 2: nodeTx.Buf[2] = (output_current >> 8) & 0xFF; nodeTx.Buf[3] = output_current & 0xFF; break;
    case 3: nodeTx.Buf[4] = (output_current >> 8) & 0xFF; nodeTx.Buf[5] = output_current & 0xFF; break;
    case 4: nodeTx.Buf[6] = (output_current >> 8) & 0xFF; nodeTx.Buf[7] = output_current & 0xFF; break;
  }
}
