# RoboMaster-CAN-SPI
GR-Peachで使用したRoboMaster C620をMCP2515を用いてSPI通信するライブラリ

# 目的
このプログラムはProject Rの関東春ロボ2025でロボマスモータをGR-PEACHで回すため製作しました。
基本的な動作は滝沢ロボティクス様を参考にしました。
https://takizawa-robotics.com/2024/09/02/m3508-arduino-mcp2515/


また、このプログラムを動作するにはMCP2515のライブラリが必要です。
GR-PEACHではピンに対してpinModeでinputをした状態でHIGH,LOWをdigitalWriteすると停止する為、一部修正したものを使用しています。

MCPから受信/送信をする方法はLOOP内の
  bus.update();
  bus.set();
が対応しています。



