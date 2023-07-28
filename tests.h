#include "ShroomGroom.h"
#include <Arduino.h>

bool test(String name, ShroomGroom* shroom, float minHumidity, float maxHumidity, float currentHumidity1, float currentHumidity2, bool fanExpected, bool diffusorExpected){

    Serial.begin(9600);

    shroom->setMinHumidity(minHumidity);
    shroom->setMaxHumidity(maxHumidity);
  
    shroom->setCurrent(20, 23, currentHumidity1, currentHumidity2);

    Serial.println("TEST: " + name);
    Serial.println("minHumidity: " + String(minHumidity));
    Serial.println("maxHumidity: " + String(maxHumidity));
    Serial.println("currentHumidity1: " + String(currentHumidity1));
    Serial.println("currentHumidity2: " + String(currentHumidity2));

    bool fan = shroom->shouldFan();
    bool diffuse = shroom->shouldDiffuse();

    Serial.println("Fan Expected: " + String(fanExpected) + " / fan: " + String(fan));
    Serial.println("Diffusior Expected: " + String(diffusorExpected) + " / diffusor: " + String(diffuse));

    if (fan == fanExpected && diffuse == diffusorExpected){
      return true;
    }
    return false;  
}


void runTests(ShroomGroom* shroom){
  test ("everything in range", shroom, 80, 90, 85, 85, false, false);
  delay(1000);
  test ("humidity1 too low", shroom, 80, 90, 79.02, 82, false, true);
  delay(1000);
  test ("humidity1 and humidity2 too low", shroom, 80, 90, 79.02, 60.987, false, true); 
  delay(1000);
  test ("humidity2 too high, humidity1 too low", shroom, 80, 90, 75.9, 93, true, true);
  delay(1000);
  test ("humidity too high on both sensors", shroom, 80, 90, 98, 92, true, false); 
}