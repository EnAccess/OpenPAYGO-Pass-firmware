#include "Arduino.h"
#define SensorSmoothingCycles 5

extern int CURRENT_ANALOG_PIN; 
extern int VOLTAGE_ANALOG_PIN;

extern int BatteryVoltage;
extern int BatteryCurrent;

void sensors_reading(int *BatteryVoltage, int *BatteryCurrent);