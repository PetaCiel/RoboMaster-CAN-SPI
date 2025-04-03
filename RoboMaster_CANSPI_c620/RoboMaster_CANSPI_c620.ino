#include <mcp_can.h>
#include <SPI.h>
#include "RoboMaster.h"

//CanBus lib instance
CanBus bus { &SPI , 8 };
//RoboMaster lib instance
RoboMasterC620 motor1 { &bus , 1 };
RoboMasterC620 motor2 { &bus , 2 };
RoboMasterC620 motor3 { &bus , 3 };
RoboMasterC620 motor4 { &bus , 4 };



void setup(){
  Serial.begin(115200);
  //Bus Initialize
  bus.init();
}

void loop(){
  //busの更新
  bus.update();

  //各motorの更新
  motor1.update();
  motor2.update();
  motor3.update();
  motor4.update();


  Serial.print("MotorID:1 Ang=");
  Serial.print(motor1.getAngle());
  Serial.print(", MotorID:2 RPM=");
  Serial.print(motor2.getRPM());
  Serial.print(", MotorID:3 AMP=");
  Serial.print(motor3.getAmp());
  Serial.print(", MotorID:4 TEMP=");
  Serial.print(motor4.getTemp());
  Serial.println();


  //SetCurrent関数 電流値を指定して格納
  motor1.setCurrent( 0.6 );
  motor2.setCurrent( 0.6 );
  motor3.setCurrent( 0.6 );
  motor4.setCurrent( 0.6 );

  bus.set();
  delay(10);
}


