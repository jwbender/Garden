#include <EEPROM.h>
//Datastorage
#include <Wire.h>
#include <Arduino.h>
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>

RTC_Millis rtc;

Sd2Card card;
SdVolume volume;
SdFile root;

#define SYNC_INTERVAL 1000 // mills between calls to flush() - to write data to the card
uint32_t syncTime = 0; // time of last sync()
const int chipSelect = 10;    
File logfile;

int soil1 =0;
int soil2 =0;
int soil3 =0;

#include <TimeLib.h>
unsigned long delaywater1 = 86400; //2 days faucit servo
unsigned long delaywater2 = 86400; //2 days bucket pump
unsigned long starttime1 = now();
unsigned long starttime2 = now();
unsigned long nowtime=now();

#include "DHT.h"
#define DHTPIN 5    // modify to the pin we connected
#define DHTTYPE DHT21   // AM2301 
 
DHT dht(DHTPIN, DHTTYPE);
 float h; 
 float t;

void setup() {
Serial.begin(9600);

pinMode(2, OUTPUT);
pinMode(3, OUTPUT);
pinMode(4, OUTPUT);
digitalWrite(2,LOW);
digitalWrite(3,LOW);
digitalWrite(4,LOW);

pinMode(6, OUTPUT);
pinMode(7, OUTPUT);

//EEPROM.get(eeAddress);

//Data Storage
  //RTC setup
rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
//initialize the SD card
Serial.print("Initializing SD card...");
//make sure that the default chip select pin is set to
//output, even if you don't use it:
pinMode(SS, OUTPUT);
pinMode(10, OUTPUT);
//see if the card is present and can be initialized:
if (!SD.begin(chipSelect)) {
Serial.println("Card failed, or not present");
//don't do anything more:
//while (1) ;
  }
Serial.println("card initialized.");
//create a new file
char filename[] = "LOGGER00.CSV";
for (uint8_t i = 0; i < 100; i++) {
  filename[6] = i/10 + '0';
  filename[7] = i%10 + '0';
  if (! SD.exists(filename)) {
// only open a new file if it doesn't exist
  logfile = SD.open(filename, FILE_WRITE); 
  Serial.println(filename);
  break;  // leave the loop!
  }
}
if (! logfile) {
}
Serial.print("Logging to: ");
Serial.println(filename);
Wire.begin();
int starttime1 = now();
int starttime2 = now();
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  dht.begin();
}

void loop() {
nowtime=now();
digitalWrite(2,HIGH);
digitalWrite(3,HIGH);
digitalWrite(4,HIGH);
delay(10000); //let sensors warm up for 10 secs
soil1=analogRead(A2);
soil2=analogRead(A3);
soil3=analogRead(A0);

Serial.println(soil1);
Serial.println(soil2);
Serial.println(soil3);
Serial.println("");

 h = dht.readHumidity();
 t = dht.readTemperature();
   Serial.print("Humidity: "); 
   Serial.print(h);
   Serial.print(" %\t");
   Serial.print("Temperature: "); 
   Serial.print(t);
   Serial.println(" *C");
    Serial.println(nowtime);
      Serial.println(starttime2);
/*if(soil1>=20000){
  while (soil1 >= 350){
  digitalWrite(7, LOW);
  soil1=analogRead(A0);
  delay(1000);
   }
}*/

if (nowtime>= (starttime1 + delaywater1)){
  digitalWrite(6, LOW);
  starttime1 = nowtime;
  logfile.println("1 Turned on");
  delay(30000); //Water for 30 secs
  digitalWrite(6, HIGH);
}
//pump water cycle
if (nowtime>= (starttime2 + delaywater2)){
  digitalWrite(7, LOW);
  starttime2 = nowtime;
  logfile.println("2 Turned on");
  delay(45000); //Water for 45 secs
  digitalWrite(7, HIGH);
}

  //data logger

  Serial.println("logging");
  DateTime now = rtc.now();
  logfile.print(now.year(), DEC);
  logfile.print("/");
  logfile.print(now.month(), DEC);
  logfile.print("/");
  logfile.print(now.day(), DEC);
  logfile.print(" ");
  logfile.print(now.hour(), DEC);
  logfile.print(":");
  logfile.print(now.minute(), DEC);
  logfile.print(":");
  logfile.print(now.second(), DEC);
  logfile.print(",");
  logfile.print("soil1");
  logfile.print(", ");
  logfile.print(soil1);
  logfile.print(", ");
  logfile.print("soil2");
  logfile.print(", ");
  logfile.print(soil2);
  logfile.print(", ");
  logfile.print("soil3");
  logfile.print(", ");
  logfile.print(soil3);
  logfile.print(",");
  logfile.print("Humidity: "); 
  logfile.print(", ");
  logfile.print(h);
  logfile.print(", ");
  logfile.print("Temperature: "); 
  logfile.print(", ");
  logfile.println(t);
  logfile.flush();

digitalWrite(2,LOW);
digitalWrite(3,LOW);
digitalWrite(4,LOW);
delay(3600000);
}
