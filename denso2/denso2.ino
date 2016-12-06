#include <Servo.h>

Servo servoLE;//サーボのインスタンス
Servo servoLS;//サーボのインスタンス
Servo servoRE;//サーボのインスタンス
Servo servoRS;//サーボのインスタンス

#define LE 6   //左翼エレボン
#define LS 7   //左翼スポイラー
#define RE 4   //右翼エレボン
#define RS 5   //右翼スポイラー

#define AST1 A2 //アナログスティック１（右で０）
#define AST2 A1 //アナログスティック２（上で０）
#define AST3 A3 //アナログスティック３（左で０）
#define AST4 A4 //アナログスティック４（下で０）

#define siki 25 //ジョイスティック閾値

#define EC 1340 //エレボン中央位置
#define ES 130  //エレボン可動範囲

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
  int val1 =  analogRead(AST1);//0~1023
  int val2 =  analogRead(AST2);//0~1023
  int val3 =  analogRead(AST3);//0~1023
  int val4 =  analogRead(AST4);//0~1023
  if(val2 <= 512) val2= 512;
  
  //map()を使って0~1023を544~2400に変換
  int pulseWidthLE;
  int pulseWidthRE;
  int pulseWidthLS;
  int pulseWidthRS;

  /*右スティック中央*/
  if((val3 >= 512 - siki && val3 <= 512 + siki) && (val4 >= 0 + siki && val4 <= 1023)){
    pulseWidthLE = map(val4, 0, 1023, EC - ES, EC + ES);
    pulseWidthRE = map(val4, 0, 1023, EC - ES, EC + ES);   
    servoLE.writeMicroseconds(pulseWidthLE);//左エレボン
    servoRE.writeMicroseconds(pulseWidthRE);//右エレボン
  }   
  /*右スティック左側*/
  else if((val3 >= 0 && val3 <= 512 - siki) && (val4 >= 512 - siki && val4 <= 512 + siki)){
    pulseWidthLE = map(val3, 0, 1023, EC, EC - ES);
    pulseWidthRE = map(val3, 0, 1023, EC - ES, EC);   
    servoLE.writeMicroseconds(pulseWidthLE);//左エレボン
    servoRE.writeMicroseconds(pulseWidthRE);//右エレボン
  } 
  /*右スティック右側*/
  else if((512 + siki && val3 <= 1023) && (val4 >= 512 - siki && val4 <= 512 + siki)){
    pulseWidthLE = map(val3, 0, 1023, EC - ES, EC);   
    pulseWidthRE = map(val3, 0, 1023, EC, EC - ES);
    servoLE.writeMicroseconds(pulseWidthLE);//左エレボン
    servoRE.writeMicroseconds(pulseWidthRE);//右エレボン
  }    

   /*左スティック中央*/
  if((val1 >= 512 - siki && val1 <= 512 + siki) && (val2 >= 512 + siki && val2 <= 1023)){
    pulseWidthLS =  map(val2,512,1023,1450,2300);
    pulseWidthRS =  map(val2,512,1023,1450,2300);
    servoLE.writeMicroseconds(pulseWidthLS);//左エレボン
    servoRE.writeMicroseconds(pulseWidthRS);//右エレボン
  }   
  
  /*右スティック左側*/
  else if((val1 >= 512 + siki && val1 <= 1023) && (val2 >= 512 - siki && val2 <= 512 + siki)){
    pulseWidthLE = map(val2,512,1023,1450,2300);
    pulseWidthRE = 1450;   
    servoLE.writeMicroseconds(pulseWidthLE);//左エレボン
    servoRE.writeMicroseconds(pulseWidthRE);//右エレボン
  } 

  /*右スティック右側*/
  else if((val1 >= 0 && val1 <= 512 - siki) && (val2 >= 512 - siki && val2 <= 512 + siki)){
    pulseWidthLE = 1450;
    pulseWidthRE = map(val2,512,1023,1450,2300); 
    servoLE.writeMicroseconds(pulseWidthLE);//左エレボン
    servoRE.writeMicroseconds(pulseWidthRE);//右エレボン
  } 
  
  //サーボ出力  
  servoLE.writeMicroseconds(pulseWidthLE);//左エレボン
  servoLS.writeMicroseconds(pulseWidthLS);//左スポイラー
  servoRE.writeMicroseconds(pulseWidthRE);//右エレボン
  servoRS.writeMicroseconds(pulseWidthRS);//右スポイラー
}

