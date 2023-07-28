#ifndef _SHROOMGROOM_
#define _SHROOMGROOM_

class ShroomGroom {
  public:
    void setMinHumidity(double);
    void setMaxHumidity(double);
    int getMinHumidity(void);
    int getMaxHumidity(void);
    void setCurrent(float temp1, float temp2, float humidity1, float humidity2);
    bool shouldFan();
    bool shouldDiffuse();
  private:
    float minHumidity;
    float maxHumidity;
    float currentTemp1;
    float currentTemp2;
    float currentHumidity1;
    float currentHumidity2;
    float samplingInterval;
};

#endif