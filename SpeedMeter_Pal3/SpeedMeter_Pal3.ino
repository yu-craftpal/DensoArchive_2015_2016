#include <MsTimer2.h>

unsigned long beforeTime;
unsigned long elapsedTime;
int flagHi;
int flag2;
int flag3;
double speed1;

int Trig = 47;
int Echo = 46;
int Duration;
float Distance;
int kyori;

//リードスイッチ→デジタル５ピン
#define rsw 22

const int anode_pins[] = {30, 31, 32, 33, 34, 35, 36};    // アノードに接続するArduinoのピン
const int cathode_pins1[] = {38, 39, 40};  // カソードに接続するArduinoのピン
const int cathode_pins2[] = {42, 43, 44, 45};  // カソードに接続するArduinoのピン
const int number_of_anode_pins = sizeof(anode_pins) / sizeof(anode_pins[0]);
const int number_of_cathode_pins1 = sizeof(cathode_pins1) / sizeof(cathode_pins1[0]);
const int number_of_cathode_pins2 = sizeof(cathode_pins2) / sizeof(cathode_pins2[0]);
int numbers_to_display = 0; // LEDに表示する数字を保持する変数
 
const int digits[] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00100111, // 7
  0b01111111, // 8
  0b01101111, // 9
};

 
// 1桁の数字(n)を表示する
void display_number (int n) {
  for (int i = 0; i < number_of_anode_pins; i++) {
    digitalWrite(anode_pins[i], digits[n] & (1 << i) ? HIGH : LOW);
  }  
}
 
// アノードをすべてLOWにする
void clear_segments() {
  for (int j = 0; j < number_of_anode_pins; j++) {
    digitalWrite(anode_pins[j], LOW);
  }
}
 
void display_numbers () {
  int n = numbers_to_display;  // number_to_displayの値を書き換えないために変数にコピー
  int n2 = millis() / 1000;
  for (int i = 0; i < number_of_cathode_pins1; i++) {
    digitalWrite(cathode_pins1[i], LOW);
    display_number(n % 10); // 最後の一桁を表示する
    delayMicroseconds(50);
    clear_segments();
    digitalWrite(cathode_pins1[i], HIGH);
    n = n / 10; // 10で割る
  }
  /*
  for (int i = 0; i < number_of_cathode_pins2; i++) {
    digitalWrite(cathode_pins2[i], LOW);
    display_number(n2 % 10); // 最後の一桁を表示する
    delayMicroseconds(50);
    clear_segments();
    digitalWrite(cathode_pins2[i], HIGH);
    n = n2 / 10; // 10で割る
  } 
*/
}
 
void set_numbers(int n) {
  numbers_to_display = n;
}


void setup() {
  // put your setup code here, to run once:
   //初期化
  beforeTime  = 0;
  elapsedTime = 0;
  flagHi = 0;
  flag2 = 0;
  flag3 = 0;

  for (int i = 0; i < number_of_anode_pins; i++) {
    pinMode(anode_pins[i], OUTPUT);  // anode_pinsを出力モードに設定する
  }
  for (int i = 0; i < number_of_cathode_pins1; i++) {
    pinMode(cathode_pins1[i], OUTPUT);  // cathode_pinを出力モードに設定する
    digitalWrite(cathode_pins1[i], HIGH);
  }
  for (int i = 0; i < number_of_cathode_pins2; i++) {
    pinMode(cathode_pins2[i], OUTPUT);  // cathode_pinを出力モードに設定する
    digitalWrite(cathode_pins2[i], HIGH);
  }  

  pinMode(Trig,OUTPUT);
  pinMode(Echo,INPUT);
   
  // f = クロック周波数 / ( 2 * 分周比　*　( 1 + 比較レジスタの値))
  // 分周比=32, 比較レジスタの1値=255 -> f = 16000000 / (2 * 32 * 256) = 976 Hz
  OCR2A = 255; // 255クロックごとに割り込みをかける
  TCCR2B = 0b100; // 分周比を32に設定する
  bitWrite(TIMSK2, OCIE2A, 1); // TIMER2を
  
  Serial.begin(9600);  

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
    
    elapsedTime = nowTime - beforeTime;//前回パルスアップからの経過時間
    
      flagHi = 0;
      beforeTime = nowTime;//今の時間を記録する
  
      speed1 =000.00;
      double buf1 = elapsedTime / 6;
      double buf2 = (1 / buf1) * 10000;
      speed1 =  buf2;
      set_numbers(speed1);      
    


    delay(100);
    
  }
  /*
  digitalWrite(Trig,LOW);
  delayMicroseconds(1);
  digitalWrite(Trig,HIGH);
  delayMicroseconds(1);
  digitalWrite(Trig,LOW);
  Duration = pulseIn(Echo,HIGH);
  
  if (Duration>0) {
    Distance = Duration/2;
    Distance = Distance*340*100/1000000; // ultrasonic speed is 340m/s = 34000cm/s = 0.034cm/us   

    kyori=Distance;
    Serial.println(kyori);
  }
*/
  //display_numbers();

  
  

  
  //15秒経過で0km/hにする
  if(nowTime - beforeTime>5000) {elapsedTime=0.0;set_numbers(0);}

}

ISR(TIMER2_COMPA_vect)
{
  display_numbers();
  
}


