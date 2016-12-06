#include <Servo.h>

Servo servoLE;//サーボのインスタンス
Servo servoLS;//サーボのインスタンス
Servo servoRE;//サーボのインスタンス
Servo servoRS;//サーボのインスタンス

#define siki 100 //ジョイスティック閾値
#define EC 1436 //エレボン中央位置
//#define ES 130  //エレボン可動範囲
#define ES 250  //エレボン可動範囲
#define SC 1450 //スポイラー初期位置
#define SM 2300 //スポイラー可動最大値

#define LE 6   //左翼エレボン
#define LS 7   //左翼スポイラー
#define RE 4   //右翼エレボン
#define RS 5   //右翼スポイラー
#define AST1 A2 //アナログスティック１（右で０）
#define AST2 A1 //アナログスティック２（上で０）
#define AST3 A3 //アナログスティック３（左で０）
#define AST4 A4 //アナログスティック４（下で０）

//サーボに送信する値
int pulseWidthLE;
int pulseWidthRE;
int pulseWidthLS;
int pulseWidthRS;

void setup(){
  pinMode(LE,OUTPUT);
  pinMode(RE,OUTPUT);
  pinMode(LS,OUTPUT);
  pinMode(RS,OUTPUT);
  digitalWrite(LE,LOW);
  digitalWrite(RE,LOW);
  digitalWrite(LS,LOW);
  digitalWrite(RS,LOW);
  delay(1000);
  
  //サーボの信号線を３番ピンに接続
  //（PWMピン以外のピンにも接続可）
  servoLE.attach(LE);
  servoLS.attach(LS);
  servoRE.attach(RE);
  servoRS.attach(RS);

  //サーボを初期位置に設定
  pulseWidthLS = SC; 
  pulseWidthRS = SC;
  pulseWidthLE = EC;
  pulseWidthRE = EC;   

  Serial.begin(9600);
}

void loop(){

  AnalogStick();
  SerialPrint();  

}

void AnalogStick(){
  int i;
  //センサからの読取り値
  int val1 =  analogRead(AST1);//0~1023
  int val2 =  analogRead(AST2);//0~1023
  int val3 =  analogRead(AST3);//0~1023
  int val4 =  analogRead(AST4);//0~1023
  if(val2 >= 512) val2= 512;
 
  /*左スティック中央*/
  if((val1 >= 512 - siki && val1 <= 512 + siki) && (val2 >= 0 && val2 <= 512 - siki)){
    pulseWidthLS =  map(val2, 512 - siki, 0, SC, SM);
    pulseWidthRS =  map(val2, 512 - siki, 0, SC, SM);
    servoLS.writeMicroseconds(pulseWidthLS);//左スポイラー
    servoRS.writeMicroseconds(pulseWidthRS);//右スポイラー
    Serial.print("LC");
    Serial.print(":");
  }
  /*左スティック左側*/
  else if((val1 >= 512 + siki && val1 <= 1023) && (val2 >= 512 - siki && val2 <= 512 + siki)){
    pulseWidthLS = map(val1, 512 + siki, 1023, SC, SM);
    pulseWidthRS = SC;   
    servoLS.writeMicroseconds(pulseWidthLS);//左スポイラー
    servoRS.writeMicroseconds(pulseWidthRS);//右スポイラー
    Serial.print("LL");
    Serial.print(":");
  } 
  /*左スティック右側*/
  else if((val1 >= 0 && val1 <= 512 - siki) && (val2 >= 512 - siki && val2 <= 512 + siki)){
    pulseWidthLS = SC;
    pulseWidthRS = map(val1, 512 - siki, 0, SC, SM); 
    servoLS.writeMicroseconds(pulseWidthLS);//左スポイラー
    servoRS.writeMicroseconds(pulseWidthRS);//右スポイラー
    Serial.print("LR");
    Serial.print(":");
  }
  else{
    pulseWidthLS = SC; 
    pulseWidthRS = SC; 
    servoLS.writeMicroseconds(pulseWidthLS);//左スポイラー
    servoRS.writeMicroseconds(pulseWidthRS);//右スポイラー
    Serial.print("LO");
    Serial.print(":");    
  }


  /*右スティック中央*/
  if((val3 >= 512 - siki && val3 <= 512 + siki) && (val4 >= 512 - siki && val4 <= 512 + siki)){
    pulseWidthLE = EC;
    pulseWidthRE = EC;
    servoLE.writeMicroseconds(pulseWidthLE);//左エレボン
    servoRE.writeMicroseconds(pulseWidthRE);//右エレボン
    Serial.print("RC");
    Serial.print(":");
  }   
  /*右スティック上側*/
  else if((val3 >= 512 - siki && val3 <= 512 + siki) && (val4 >= 512 + siki && val4 <= 1023)){
    pulseWidthLE = map(val4, 512 + siki, 1023, EC, EC + ES);
    pulseWidthRE = map(val4, 512 + siki, 1023, EC, EC + ES);   
    servoLE.writeMicroseconds(pulseWidthLE);//左エレボン
    servoRE.writeMicroseconds(pulseWidthRE);//右エレボン
    Serial.print("RU");
    Serial.print(":");
  }
  /*右スティック下側*/
  else if((val3 >= 512 - siki && val3 <= 512 + siki) && (val4 >= 0 && val4 <= 512 - siki)){
    pulseWidthLE = map(val4, 512 - siki, 0, EC, EC - ES);
    pulseWidthRE = map(val4, 512 - siki, 0, EC, EC - ES);   
    servoLE.writeMicroseconds(pulseWidthLE);//左エレボン
    servoRE.writeMicroseconds(pulseWidthRE);//右エレボン
    Serial.print("RD");
    Serial.print(":");
  }  
  /*右スティック左側*/
  else if((val3 >= 0 && val3 <= 512 - siki) && (val4 >= 512 - siki && val4 <= 512 + siki)){
    pulseWidthLE = map(val3, 512 - siki, 0, EC, EC - ES);
    pulseWidthRE = map(val3, 512 - siki, 0, EC, EC + ES);   
    servoLE.writeMicroseconds(pulseWidthLE);//左エレボン
    servoRE.writeMicroseconds(pulseWidthRE);//右エレボン
    Serial.print("RL");
    Serial.print(":");
  } 
  /*右スティック右側*/
  else if((512 + siki && val3 <= 1023) && (val4 >= 512 - siki && val4 <= 512 + siki)){
    pulseWidthLE = map(val3, 512 + siki, 1023, EC, EC + ES);
    pulseWidthRE = map(val3, 512 + siki, 1023, EC, EC - ES);
    servoLE.writeMicroseconds(pulseWidthLE);//左エレボン
    servoRE.writeMicroseconds(pulseWidthRE);//右エレボン
    Serial.print("RR");
    Serial.print(":");
  }    
  else{
    Serial.print("RO");
    Serial.print(":");    
  }
}

void SerialPrint(){
  /*
      Serial.print(val1);
    Serial.print(":"); 
    Serial.print(val2);
    Serial.print(":");
    Serial.print(val3);
    Serial.print(":");
    Serial.print(val4);
  */
  
    Serial.print(pulseWidthLE);
    Serial.print(":"); 
    Serial.print(pulseWidthRE);
    Serial.print(":");
    Serial.print(pulseWidthLS);
    Serial.print(":");
    Serial.println(pulseWidthRS);
}

