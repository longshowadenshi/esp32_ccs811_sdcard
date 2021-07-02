#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include "ccs811.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"

String datamsg1;
String datamsg2;

char auth[] = "KCTUc7bmskcd2MAvDHpwHry32KsasEyq";
char ssid[] = "Buffalo-G-9300";
char pass[] = "trwewx6vighnk";

ccs811 ccs811(1);

void setup() {
  Wire.begin();                   
  Serial.begin(115200);
  
  //Blynk init
  Blynk.begin(auth, ssid, pass);

  //ccs811 init
  ccs811.begin();

  //sd card init
  if(!SD.begin()){
      Serial.println("Card Mount Failed");
      return;
  }
  writeFile(SD, "/CO2.txt", "CO2(ppm)|tVoc(ppb)\r\n");
}

void loop() {

  //get data from ccs811
  if(ccs811.errstatus()==ERRSTAT_OK){
  ccs811.ReadData();
  Serial.print("CO2=");
  Serial.print(ccs811.getCO2());
  Serial.print(" ppm ");
  Serial.print("     tVOC=");
  Serial.print(ccs811.gettVoc()); 
  Serial.println(" ppb");
  }

  //save data into sd card
  datamsg1=String(ccs811.getCO2());
  datamsg2=String(ccs811.gettVoc());
  appendFile(SD, "/CO2.txt", datamsg1.c_str());
  appendFile(SD, "/CO2.txt", "      |  ");
  appendFile(SD, "/CO2.txt", datamsg2.c_str());
  appendFile(SD, "/CO2.txt", "\n");

  //display CO2,tVoc value on Blynk app
  Blynk.virtualWrite(V1, ccs811.getCO2());
  Blynk.virtualWrite(V2, ccs811.gettVoc());
  delay(1000);
}
void writeFile(fs::FS &fs, const char * path, const char * message){
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    file.print(message);
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    file.print(message);
    file.close();
}
