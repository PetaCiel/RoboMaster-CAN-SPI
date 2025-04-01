/*
 * 本コード作成及び責任者  : 森 星次郎
 *       目    的         : ロボマス処理ライブラリ
 * 最  終  更  新  日  時 : 2025/02/27

 * 基本的な動作は滝沢ロボティクス氏を参考にしました
 * このライブラリを動かすにはMCP2515ライブラリが必要
 * また、このライブラリをGR-PEACHで動かすには、一部の変更を加えたMCP2515_Appendライブラリが必要
 * これはどこかで自分が投稿したはず


 * まずCanBusライブラリをインスタンス化

      CanBus bus { &SPI , 8 };

 * &SPIは使用するSPIポート　今回はSPI無印を用いる為このように記述する
 * 8はCSに使用するGPIO番号、基板資料を要確認
 * 例えばSPI1にbus_01を、SPI2にbus_02を繋ぐ場合は

      CanBus bus_01 { &SPI1 , bus_01_CS };//bus_01_CSは任意のピン
      CanBus bus_02 { &SPI2 , bus_02_CS };//bus_02_CSは任意のピン

 * また、MCPから受信、送信をする方法はLOOP内の

      //busをCanBusで宣言したと仮定
      bus.update();
      bus.set();

 * が担当
 * これよりLOOP処理内の先頭にupdateを、終端にsetを置く必要アリ


 * 次にロボマスライブラリのインスタンス化について

      RoboMasterC620 motor1 { &bus , 1 };

 * が、ロボマスのモータ定義処理である
 * { &bus , 1}は、CanBus型のbusの、1番(1~4であって0~3では無い点に注意)をmotor1として扱う事を表している
 * 例えばbus_02の4番を、motor_02_4として宣言する場合

      RoboMasterC620 motor_02_4 {&bus_02 , 4};

 * と宣言するとmotor_02_4をbus_02の4番のモータとして扱えるよ
 * 各モータには.getRPM()とかの関数が扱えるはず、これらで各モータのﾊﾟﾗﾒｰﾀが取得出来る
 * 出力する際はsetCurrent関数が使えるはず、電流値で-26.0A~26.0Aの値を投げることで電流値での回転指令を出せる
 * また、

      bus.set();

 * の中身は送信データ8バイトをかたっぱしから送っている、ので

      motor1.setCurrent( motorCmd_1 );
      bus.set(); //BE EF 00 00 00 00 00 00の様に、motor1の指令値だけ送信
      motor2.setCurrent( motorCmd_2 );
      bus.set(); //00 00 FA CE 00 00 00 00の様に、motor2の指令値だけ送信
      motor3.setCurrent( motorCmd_3 );
      bus.set(); //00 00 00 00 DE AD 00 00の様に、motor3の指令値だけ送信
      motor4.setCurrent( motorCmd_4 );
      bus.set(); //00 00 00 00 00 00 CA FEの様に、motor4の指令値だけ送信

 * みたいなことすると非効率の極みであるため、

      motor1.setCurrent( motorCmd_1 );
      motor2.setCurrent( motorCmd_2 );
      motor3.setCurrent( motorCmd_3 );
      motor4.setCurrent( motorCmd_4 );
      bus.set(); //BE EF FA CE DE AD CA FEが一気に送信

 * みたいに書いた方が効率的かも


 * また、さも5,6,7,8番目のモータを扱えそうに見えるけど、読むとわかるけど0x200しか定義していないから扱えるモータは4つのみ


 * 春ロボ用のRoboMaster C620プログラムの改_2だよん
 * 回転部分を自動でPIDするように変更した
 */

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

double motorCmd=0.8;




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
  motor1.setCurrent( 0.4 );
  motor2.setCurrent( 0.4 );
  motor3.setCurrent( 0.4 );
  motor4.setCurrent( 0.4 );

  bus.set();
  delay(10);
}


