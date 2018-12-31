#include <TimeLib.h>
//#include "U8glib.h"
//U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI 
float soil =0;

//define relays 
int no1=2;
int no2=3;
int no3=4;
int nc1=5;

unsigned long delayon =  50400;// 14 hours
unsigned long delayoff = 36000;// 10 hours

unsigned long starttime = now();
unsigned long nowtime = now();

int switchstate = 0;

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,40);

void setup() {
Serial.begin(9600);
pinMode(no1, OUTPUT);
pinMode(no2, OUTPUT);
pinMode(no3, OUTPUT);
pinMode(nc1, OUTPUT);

digitalWrite(no1, HIGH); //outlet energized
digitalWrite(no2, HIGH);
digitalWrite(no3, HIGH);
digitalWrite(nc1, HIGH);

lcd.init();
lcd.backlight();

}
void loop() {
  nowtime = now()+27000;
if (nowtime>= (starttime + delayon) & switchstate==1){
  digitalWrite(no1, HIGH);
  digitalWrite(no2, HIGH);
  digitalWrite(no3, HIGH);
  switchstate=0;
  starttime = nowtime;
  Serial.println("turned off  ");
  Serial.println(nowtime);
}
if (nowtime>= (starttime + delayoff) & switchstate==0){
  digitalWrite(no1, LOW);
  digitalWrite(no2, LOW);
  digitalWrite(no3, LOW);
  switchstate=1;
  starttime = nowtime;
  Serial.println("turned on  ");
  Serial.print(nowtime);
}
Serial.print("Now:  ");
Serial.println(nowtime);
Serial.print("starttime:  ");
Serial.println(starttime);
delay(2000);
 /* u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );*/
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Now: ");
  lcd.print(nowtime);
  lcd.setCursor(0,1);
  lcd.print("Start: ");
  lcd.print(starttime);
}
