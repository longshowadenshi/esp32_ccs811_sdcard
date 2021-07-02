#define STATUS_REG 0x00 
#define MEAS_MODE_REG 0x01 
#define ALG_RESULT_DATA 0x02 
#define ENV_DATA 0x05 
#define HW_ID_REG 0x20 
#define ERROR_ID_REG 0xE0 
#define APP_START_REG 0xF4
#define SW_RESET 0xFF
#define HW_ID 0x81

#define CCS_811_ADDRESS 0x5A 
#define DRIVE_MODE_IDLE 0x0 
#define DRIVE_MODE_1SEC 0x10 
#define DRIVE_MODE_10SEC 0x20 
#define DRIVE_MODE_60SEC 0x30 

#define APP_VALID 0x10
#define FW_MODE 0x80

#define ERRSTAT_OK (APP_VALID|FW_MODE)

#ifndef ccs811_H
#define ccs811_H
#include <Arduino.h>

class ccs811
{
  public:
  ccs811(uint8_t Mode);
  void begin();
  void ReadHW();
  void ReadAppvalid();
  void Appstart();
  void ReadFWmode();
  void Meas_mode(uint8_t MeasMode);
  void ReadData();
  uint16_t getCO2();
  uint16_t gettVoc();
  uint8_t errstatus();

  private:
  uint8_t errstat;
  uint8_t BootMode[4]={0x11,0xE5,0x72,0x8A};
  uint8_t _Mode;
  uint8_t Data[3];
  uint8_t _Data[4];
  uint16_t CO2;
  uint16_t tVoc;
};

#endif
