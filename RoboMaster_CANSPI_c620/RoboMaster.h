#ifndef RoboMaster_H
#define RoboMaster_H
#include <Arduino.h>
#include <mcp_can.h>
#include <SPI.h>

//CanNode Struct
typedef struct{
  uint32_t Id;
  byte Buf[8];  
}CanNode;

//CanBus Class
class CanBus{
public:
//CanBus_construct
  CanBus(SPIClass *CB_SPI, INT8U CB_CS);
//public_function
  void init();
  void update();
  void set();
  byte SubRxBuf[4][8]={0};
private:
  MCP_CAN RM_CAN;
  unsigned char len = 0;
  long Pre_millis;
  int RxId;
  byte MainTxBuf[8]={0};
  byte MainRxBuf[8]={0};
};

//RoboMasterC620Class
class RoboMasterC620{
public:
  RoboMasterC620(CanBus* bus, int id);
  int getAngle();
  int getRPM();
  int getAmp();
  int getTemp();
  void update();
  void setCurrent(double current);
private:
  int Id;
  int rxBuf[8]={0};
  int16_t angle;
  int16_t rpm;
  int16_t amp;
  int16_t temp;
  CanBus *MYBUS;
  int16_t floatmapping(double x, double in_min, double in_max, int16_t out_min, int16_t out_max);
};
#endif
