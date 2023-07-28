#include <Arduino.h>
#include "hw_interface.h"
#include <DHT.h>

#define sensor1_PIN 2
#define sensor2_PIN 4
#define fan_PIN 5
#define humidifier_PIN 15

#define DHTTYPE DHT11   // DHT 11

DHT dht1(sensor1_PIN, DHTTYPE);
DHT dht2(sensor2_PIN, DHTTYPE);

struct sensor {     
  double temperature;
  double humidity;
};

sensor sensor1 = { 255 , 0 };
sensor sensor2 = { 0, 255  }; 

void initHw(){
  dht1.begin();
  delay(1000);
  dht2.begin();
  delay(1000);
}

float readSensor1Temp(void)
{
  float result = dht1.readTemperature();
  return result;
  delay(100);
}


float readSensor1Humidity(void)
{
  float result = dht1.readHumidity();
  return result;
  delay(100);
}

float readSensor2Temp(void)
{
  float result = dht2.readTemperature();
  return result;
  delay(100);
}


float readSensor2Humidity(void)
{
  float result = dht2.readHumidity();
  return result;
  delay(100);
}

void activateFan(void)
{
  pinMode(fan_PIN, OUTPUT);
  digitalWrite(fan_PIN,1);
}

void deactivateFan(void)
{
  pinMode(fan_PIN, OUTPUT);
  digitalWrite(fan_PIN,0);
}

void activateHumidifier(void)
{
  pinMode(humidifier_PIN, OUTPUT);
  if (readHumidifierState() == 0)
  {
    digitalWrite(humidifier_PIN,0);
    delay(1000);
    digitalWrite(humidifier_PIN,1);
  }
  
}

void deactivateHumidifier(void)
 {
    pinMode(humidifier_PIN, OUTPUT);
   if (readHumidifierState() == 1)
    {
      digitalWrite(humidifier_PIN,1);
      delay(2000);
      digitalWrite(humidifier_PIN,0);
    }
 }

int readHumidifierState(void) 
{
  int humidifierRunning = 0;
  
  for (int i=0; i<20; i++) 
  {
      
    int led = analogRead(A0);
    if (led > 100) {
      humidifierRunning = 1;
      delay(100);
    }
  }
  return humidifierRunning;
}

