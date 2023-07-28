#include "ShroomGroom.h"
#include <algorithm>
#include <Arduino.h>
using namespace std;

void ShroomGroom::setMinHumidity(double temp) {
  this->minHumidity = temp;
  Serial.print("minHumidity set to: ");
  Serial.println(temp);
}

void ShroomGroom::setMaxHumidity(double temp){
  this->maxHumidity = temp;
  Serial.print("maxHumidity set to: ");
  Serial.println(temp);
}

int ShroomGroom::getMinHumidity() {
  return (int) this->minHumidity;
}

int ShroomGroom::getMaxHumidity(){
  return (int) this->maxHumidity;
}

//for testing only
void ShroomGroom::setCurrent(float temp1, float temp2, float humidity1, float humidity2){
    this->currentTemp1 = temp1;
    this->currentTemp2 = temp2;
    this->currentHumidity1 = humidity1;  
    this->currentHumidity2 = humidity2;
}

bool ShroomGroom::shouldFan(){
  float highestHumidity = std::max(this->currentHumidity1, this->currentHumidity2);
  bool result = (highestHumidity > this->maxHumidity) ? true : false;
  return result;
}

bool ShroomGroom::shouldDiffuse(){
  float lowestHumidity = std::min(this->currentHumidity1, this->currentHumidity2);
  bool result = (lowestHumidity < this->minHumidity) ? true : false;
  return result;
}


