
#ifndef HW_INTERFACE_H
#define HW_INTERFACE_H

void initHw();

float readSensor1Temp(void);
float readSensor1Humidity(void);
 
float readSensor2Temp(void);
float readSensor2Humidity(void);

void activateFan(void);
void deactivateFan(void);

void activateHumidifier(void);
void deactivateHumidifier(void);

int readHumidifierState(void);
#endif