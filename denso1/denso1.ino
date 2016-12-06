#include <Servo.h>

Servo servoLE;//サーボのインスタンス
Servo servoLS;//サーボのインスタンス
Servo servoRE;//サーボのインスタンス
Servo servoRS;//サーボのインスタンス

#define LE 6   //左翼エレボン
#define LS 7   //左翼スポイラー
#define RE 4   //右翼エレボン
#define RS 5   //右翼スポイラー

#define AST1 A2 //アナログスティック１（上で０）
#define AST2 A1 //アナログスティック２（右で０）
#define AST3 A3 //アナログスティック３（左で０）
#define AST4 A4 //アナログスティック４（下で０）

#define siki 25

void setup(){
  
  delay(1000);
  //サーボの信号線を３番ピンに接続
  //（PWMピン以外のピンにも接続可）
  Serial.begin(9600);
  servoLE.attach(LE);
  servoLS.attach(LS);
  servoRE.attach(RE);
  servoRS.attach(RS);
}

void loop(){
  int i;
  //センサからの読取り値
  int valLE =  analogRead(AST1);//0~1023
  int valLS =  analogRead(AST2);//0~1023
  int valRE =  analogRead(AST3);//0~1023
  int valRS =  analogRead(AST4);//0~1023
  if(valLS <= 512) valLS = 512;
  if(valRS <= 512) valRS = 512;
  
  //map()を使って0~1023を544~2400に変換
  int pulseWidthLE = map(valLE,0,1023,1340,1600);
  int pulseWidthLS = map(valLS,512,1023,1450,2300);
  int pulseWidthRE = map(valRE,0,1023,1340,1600);
  int pulseWidthRS = map(valRS,512,1023,1450,2300);
  
  //サーボ出力  
  servoLE.writeMicroseconds(pulseWidthLE);//変換したパルス幅値を代入：544~2400
  servoLS.writeMicroseconds(pulseWidthLS);//変換したパルス幅値を代入：544~2400
  servoRE.writeMicroseconds(pulseWidthRE);//変換したパルス幅値を代入：544~2400
  servoRS.writeMicroseconds(pulseWidthRS);//変換したパルス幅値を代入：544~2400
}

