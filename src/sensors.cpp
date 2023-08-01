#include "sensors.hpp"

int CURRENT_ANALOG_PIN  = A1;
int VOLTAGE_ANALOG_PIN = A2;

int BatteryVoltage;
int BatteryCurrent;

void sensors_reading(int *BatteryVoltage, int *BatteryCurrent){
  //Smoothed average
  int BatVoltBuffer = 0;
  int BatCurrBuffer = 0;
  for(int i = 0; i < SensorSmoothingCycles; i++) {
    BatVoltBuffer += analogRead(VOLTAGE_ANALOG_PIN);
    BatCurrBuffer += analogRead(CURRENT_ANALOG_PIN);
  }
  *BatteryVoltage = BatVoltBuffer/SensorSmoothingCycles;
  *BatteryCurrent = BatCurrBuffer/SensorSmoothingCycles;
}