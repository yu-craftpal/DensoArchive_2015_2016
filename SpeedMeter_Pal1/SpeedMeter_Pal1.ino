#include <Wire.h>
#include "SO2002A_I2C.h"
#include <MsTimer2.h>
SO2002A_I2C oled(0x3c);

unsigned long beforeTime;
unsigned long elapsedTime;
int flagHi;
int flag2;
int flag3;
double speed1;

//タイヤ直径
#define taiya 685.8

//スイッチ→デジタル２，３，４ピン
#define sw1 2
#define sw2 3
#define sw3 4
//リードスイッチ→デジタル５ピン
#define rsw 5

byte smiley[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};

void setup() {
  // put your setup code here, to run once:
   //初期化
  beforeTime  = 0;
  elapsedTime = 0;
  flagHi = 0;
  flag2 = 0;
  flag3 = 0;
  
  //LCDの準備
  oled.begin(16, 2);
  oled.createChar(0, smiley);
  oled.noBlink();
  oled.noCursor();
  oled.setContrastControl(255);
  oled.clear();

  oled.print("HELLO!");
  delay(1000);
  oled.clear();
  oled.print("SPD: ");
  oled.setCursor(8,0); 
  oled.print("[rpm]");
  Serial.begin(9600);  

  //スレッド設定
  MsTimer2::set(100, send);
  MsTimer2::start();
}

void loop() {

  unsigned long nowTime;
  int digital0 = digitalRead(rsw);//AD0取得
  nowTime = millis();//起動してからの時間取得

  //パルスHIGH
  if (digital0 == 1 && flagHi == 0){
    flagHi = 1;
    delay(100);
  }
    
  //パルスLOW
  if (digital0 == 0 && flagHi == 1){
    flagHi = 0;
    elapsedTime = nowTime - beforeTime;//前回パルスアップからの経過時間
    beforeTime = nowTime;//今の時間を記録する

      MsTimer2::set(100, send);
      MsTimer2::start();      
      oled.display();
      oled.setCursor(8,0); 
      oled.print("[rpm]");      
      delay(100);
    
  }
  
  //30秒経過で0km/hにする
  if(nowTime - beforeTime>30000){
    elapsedTime=0.0;
  }

  
  if(flag2 == 0 && flag3 == 1){
    //LCDに表示
    //５文字の変数を作成
    char speed2[7] = "";
    //総文字数５桁、小数点２桁に変換
    dtostrf(speed1, 4, 0, speed2);
    oled.setCursor(4,0); 
    oled.print(speed2);
    
    flag3 = 0;
  }
}

void send(){
  speed1 =000.00;
  
  double buf1 = elapsedTime / 6;
  double buf2 = (1 / buf1) * 10000;
 
  Serial.print(elapsedTime);
  Serial.print(" : "); 
  Serial.print(buf1);
  Serial.print(" : ");
  Serial.println(buf2);
  
  //時速に変換
  if(elapsedTime != 0.0)speed1 =  buf2;
  flag3 = 1;
}

