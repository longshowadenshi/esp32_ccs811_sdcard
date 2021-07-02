#include "ccs811.h"
#include <Wire.h>

ccs811::ccs811(uint8_t Mode)
{
  this->_Mode=Mode;
}
void ccs811::begin()
{
  Wire.beginTransmission(CCS_811_ADDRESS);
  Wire.write(SW_RESET);
  for(int i=0;i<4;i++)
  {
    Wire.write(BootMode[i]);
  }
  Wire.endTransmission();
  delay(10);

  ReadHW();
}
void ccs811::ReadHW()
{
  Wire.beginTransmission(CCS_811_ADDRESS);
  Wire.write(HW_ID_REG);
  Wire.endTransmission();
  delay(10);

  Wire.requestFrom(CCS_811_ADDRESS,1);
  if(Wire.available()== 1)
  {
    Data[0]=Wire.read();
  }
  if((Data[0])== HW_ID)
  {
    Serial.println("Hardware id ok");
    ReadAppvalid();
    
  }
  else
  {
    Serial.println("Hardware id isnt ok");
  }
}
void ccs811::ReadAppvalid()
{
  Wire.beginTransmission(CCS_811_ADDRESS);
  Wire.write(STATUS_REG);
  Wire.endTransmission();
  delay(10);
  
  Wire.requestFrom(CCS_811_ADDRESS,1);
  if(Wire.available()==1)
  {
    Data[1]=Wire.read();
  }

  int status_bit=bitRead(Data[1],4);
  if(status_bit==1)
  {
    Serial.println("app valid");
    Appstart();
    errstat|=APP_VALID;
  }
  else
  {
    Serial.println("app invalid");
  }
}
void ccs811::Appstart()
{
  Wire.beginTransmission(CCS_811_ADDRESS);
  Wire.write(APP_START_REG);
  Wire.endTransmission();
  delay(10);
  ReadFWmode();
}

void ccs811::ReadFWmode()
{
  Wire.beginTransmission(CCS_811_ADDRESS);
  Wire.write(STATUS_REG);
  Wire.endTransmission();
  delay(10);

  Wire.requestFrom(CCS_811_ADDRESS,1);
  if(Wire.available()==1)
  {
    Data[2]=Wire.read();
  }
  int status_bit=bitRead(Data[2],7);
  if(status_bit==1)
  {
    Serial.println("FW mode ok");
    Meas_mode(_Mode);
    errstat|=FW_MODE;
  }
  else
  {
    Serial.println("FW mode is not ok");
  }
}

void ccs811::Meas_mode(uint8_t MeasMode)
{
  Wire.beginTransmission(CCS_811_ADDRESS);
  Wire.write(MEAS_MODE_REG);
  switch(MeasMode)
  {
    case 0:
      Wire.write(DRIVE_MODE_IDLE);
      break;
    case 1:
      Wire.write(DRIVE_MODE_1SEC);
      break;
    case 2:
      Wire.write(DRIVE_MODE_10SEC);
      break;
    case 3:
      Wire.write(DRIVE_MODE_60SEC);
      break;
  }
  Wire.endTransmission();
  delay(10);
}

void ccs811::ReadData()
{
  Wire.beginTransmission(CCS_811_ADDRESS);
  Wire.write(ALG_RESULT_DATA);
  Wire.endTransmission();
  delay(10);

  Wire.requestFrom(CCS_811_ADDRESS,4);
  if(Wire.available()==4)
  {
    _Data[0]=Wire.read();
    _Data[1]=Wire.read();
    _Data[2]=Wire.read();
    _Data[3]=Wire.read();
  }
  CO2=((uint16_t)_Data[0]<<8)|_Data[1];
  tVoc=((uint16_t)_Data[2]<<8)|_Data[3];
  getCO2();
  gettVoc();
}

uint16_t ccs811::getCO2()
{
  return CO2;
}


uint16_t ccs811::gettVoc()
{
  return tVoc;
}
uint8_t ccs811::errstatus()
{
  return errstat;
}
