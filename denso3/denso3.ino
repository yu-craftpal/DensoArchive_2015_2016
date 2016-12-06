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

#define siki 50 //ジョイスティック閾値

#define EC 1340 //エレボン中央位置
#define ES 130  //エレボン可動範囲
#define SC 1450 //スポイラー初期位置

  //map()を使って0~1023を544~2400に変換
int pulseWidthLE;
int pulseWidthRE;
int pulseWidthLS;
int pulseWidthRS;

void setup(){
  
  delay(1000);
  //サーボの信号線を３番ピンに接続
  //（PWMピン以外のピンにも接続可）
  Serial.begin(9600);
  servoLE.attach(LE);
  servoLS.attach(LS);
  servoRE.attach(RE);
  servoRS.attach(RS);

  pulseWidthLS = SC; 
  pulseWidthRS = SC; 
      
}

void loop(){
  int i;
  //センサからの読取り値
  int val1 =  analogRead(AST1);//0~1023
  int val2 =  analogRead(AST2);//0~1023
  int val3 =  analogRead(AST3);//0~1023
  int val4 =  analogRead(AST4);//0~1023
  if(val2 <= 512) val2= 512;
 

  /*左スティック中央*/
  if((val1 >= 512 - siki && val1 <= 512 + siki) && (val2 >= 512 + siki && val2 <= 1023)){
    pulseWidthLS =  map(val2,512,1023,SC,2300);
    pulseWidthRS =  map(val2,512,1023,SC,2300);
    servoLS.writeMicroseconds(pulseWidthLS);//左スポイラー
    servoRS.writeMicroseconds(pulseWidthRS);//右スポイラー
    Serial.print("LC");
    Serial.print(":");
  }
  /*左スティック左側*/
  else if((val1 >= 512 + siki && val1 <= 1023) && (val2 >= 512 - siki && val2 <= 512 + siki)){
    pulseWidthLS = map(val1,0,1023,SC,2300);
    pulseWidthRS = SC;   
    servoLS.writeMicroseconds(pulseWidthLS);//左スポイラー
    servoRS.writeMicroseconds(pulseWidthRS);//右スポイラー
    Serial.print("LL");
    Serial.print(":");
  } 
  /*左スティック右側*/
  else if((val1 >= 0 && val1 <= 512 - siki) && (val2 >= 512 - siki && val2 <= 512 + siki)){
    pulseWidthLS = SC;
    pulseWidthRS = map(val1,0,1023,2300,SC); 
    servoLS.writeMicroseconds(pulseWidthLS);//左スポイラー
    servoRS.writeMicroseconds(pulseWidthRS);//右スポイラー
    Serial.print("LR");
    Serial.print(":");
  }
  else{
    pulseWidthLS = SC; 
    pulseWidthRS = SC; 
    Serial.print("LO");
    Serial.print(":");    
  }


  /*右スティック中央*/
  if((val3 >= 512 - siki && val3 <= 512 + siki) && (val4 >= 0 + siki && val4 <= 1023)){
    pulseWidthLE = map(val4, 0, 1023, EC - ES, EC + ES);
    pulseWidthRE = map(val4, 0, 1023, EC - ES, EC + ES);   
    servoLE.writeMicroseconds(pulseWidthLE);//左エレボン
    servoRE.writeMicroseconds(pulseWidthRE);//右エレボン
    Serial.print("RC");
    Serial.print(":");
  }   
  /*右スティック左側*/
  else if((val3 >= 0 && val3 <= 512 - siki) && (val4 >= 512 - siki && val4 <= 512 + siki)){
    pulseWidthLE = map(val3, 0, 1023, EC + ES, EC - ES);
    pulseWidthRE = map(val3, 0, 1023, EC - ES, EC + ES);   
    servoLE.writeMicroseconds(pulseWidthLE);//左エレボン
    servoRE.writeMicroseconds(pulseWidthRE);//右エレボン
    Serial.print("RL");
    Serial.print(":");
  } 
  /*右スティック右側*/
  else if((512 + siki && val3 <= 1023) && (val4 >= 512 - siki && val4 <= 512 + siki)){
    pulseWidthLE = map(val3, 0, 1023, EC + ES, EC - ES);
    pulseWidthRE = map(val3, 0, 1023, EC - ES, EC + ES);
    servoLE.writeMicroseconds(pulseWidthLE);//左エレボン
    servoRE.writeMicroseconds(pulseWidthRE);//右エレボン
    Serial.print("RR");
    Serial.print(":");
  }    
  else{
    Serial.print("RO");
    Serial.print(":");    
  }

    /*
    Serial.print(val1);
    Serial.print(":"); 
    Serial.print(val2);
    Serial.print(":");
    Serial.print(val3);
    Serial.print(":");
    Serial.println(val4);
  */
  
    Serial.print(pulseWidthLE);
    Serial.print(":"); 
    Serial.print(pulseWidthRE);
    Serial.print(":");
    Serial.print(pulseWidthLS);
    Serial.print(":");
    Serial.println(pulseWidthRS);
           
  //サーボ出力  
  //servoLE.writeMicroseconds(pulseWidthLE);//左エレボン
  //servoLS.writeMicroseconds(pulseWidthLS);//左スポイラー
  //servoRE.writeMicroseconds(pulseWidthRE);//右エレボン
  //servoRS.writeMicroseconds(pulseWidthRS);//右スポイラー
}

