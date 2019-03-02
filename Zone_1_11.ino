
#include <TimeLib.h>
#include <VirtualWire.h>

double tdsValue,distance,timestamp;
double sensorArray[3] = {};
const int receive_pin = 11;

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

int switchstate = 1;

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,40);

void setup() {
Serial.begin(9600);
pinMode(no1, OUTPUT);
pinMode(no2, OUTPUT);
pinMode(no3, OUTPUT);
pinMode(nc1, OUTPUT);

digitalWrite(no1, LOW); //outlet energized
digitalWrite(no2, LOW);
digitalWrite(no3, LOW);
digitalWrite(nc1, HIGH);

lcd.init();
lcd.backlight();

vw_set_rx_pin(receive_pin);
vw_set_ptt_inverted(true);
vw_setup(2000);
vw_rx_start();
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
    uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  // Non-blocking
  if (vw_get_message(buf, &buflen))
  {
    memcpy(sensorArray, buf, buflen);
    digitalWrite(13, true);
   tdsValue= sensorArray[0];// == x;
   distance= sensorArray[1]; //== y;
   timestamp = sensorArray[2]; 
    Serial.print("X: ");
    Serial.println(tdsValue);
    Serial.print("Y: ");
    Serial.println(distance);
    Serial.print("z: ");
    Serial.println(timestamp);
    // Turn off light to and await next message
    digitalWrite(13, false);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Now: ");
  lcd.print(nowtime);
  lcd.setCursor(0,1);
  lcd.print("start: ");
  lcd.print(starttime);
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TDS: ");
  lcd.print(tdsValue);
  lcd.setCursor(0,1);
  lcd.print("Dist: ");
  lcd.print(distance);
  delay(2000);
}
