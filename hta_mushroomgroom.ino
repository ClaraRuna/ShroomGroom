// MQTT
#include <SPI.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

//Low Power sleep
#include <Adafruit_SleepyDog.h>

// Network
#include <Ethernet.h>
#include <ESP8266WiFi.h>

//DHT
#include <DHT.h>

#include "Config.h"
#include "ShroomGroom.h"
#include "tests.h"
#include "hw_interface.h"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USER, MQTT_PASSWORD);

ShroomGroom shroom = ShroomGroom();

Adafruit_MQTT_Publish sensor1Temp = Adafruit_MQTT_Publish(&mqtt, "/shroomgroom/sensor1temp", MQTT_QOS_1);
Adafruit_MQTT_Publish sensor2Temp = Adafruit_MQTT_Publish(&mqtt, "/shroomgroom/sensor2temp", MQTT_QOS_1);
Adafruit_MQTT_Publish sensor1Humidity = Adafruit_MQTT_Publish(&mqtt, "/shroomgroom/sensor1humidity", MQTT_QOS_1);
Adafruit_MQTT_Publish sensor2Humidity = Adafruit_MQTT_Publish(&mqtt, "/shroomgroom/sensor2humidity", MQTT_QOS_1);
Adafruit_MQTT_Publish fan = Adafruit_MQTT_Publish(&mqtt, "/shroomgroom/fan", MQTT_QOS_1);
Adafruit_MQTT_Publish diffusor = Adafruit_MQTT_Publish(&mqtt, "/shroomgroom/diffusor", MQTT_QOS_1);
Adafruit_MQTT_Publish publishConfiguredMinHumidity = Adafruit_MQTT_Publish(&mqtt, "/shroomgroom/configuredMinHumidity", MQTT_QOS_1);
Adafruit_MQTT_Publish publishConfiguredMaxHumidity = Adafruit_MQTT_Publish(&mqtt, "/shroomgroom/configuredMaxHumidity", MQTT_QOS_1);

// publish on reboot
Adafruit_MQTT_Publish reboot = Adafruit_MQTT_Publish(&mqtt, "/shroomgroom/reboot", MQTT_QOS_1);

//subscribe to humidity bounds reconfiguration
Adafruit_MQTT_Subscribe setMinHumidity = Adafruit_MQTT_Subscribe(&mqtt, "/shroomgroom/setMinHumidity", MQTT_QOS_1);
Adafruit_MQTT_Subscribe setMaxHumidity = Adafruit_MQTT_Subscribe(&mqtt, "/shroomgroom/setMaxHumidity", MQTT_QOS_1);

void MQTT_connect();


void setup(){
  Serial.begin(9600);
  
  // init shroom with default humidities
  shroom.setMinHumidity(MIN_HUMIDITY);
  shroom.setMaxHumidity(MAX_HUMIDITY);

  //runTests(&shroom);
  connectWiFi();

  setMinHumidity.setCallback(setMinHumidityCallback);
  setMaxHumidity.setCallback(setMaxHumidityCallback);
  mqtt.subscribe(&setMinHumidity);
  mqtt.subscribe(&setMaxHumidity);

  MQTT_connect();

  // indicate reboot to mqtt subscribers
  reboot.publish(1); 

  initHw();
}

void loop(){

  MQTT_connect();

  float temp1 = readSensor1Temp();
  float temp2 = readSensor2Temp();
  float humidity1 = readSensor1Humidity();
  float humidity2 = readSensor2Humidity();

  publishConfiguredMinHumidity.publish(shroom.getMinHumidity());
  publishConfiguredMaxHumidity.publish(shroom.getMaxHumidity());

  sensor1Temp.publish((int)temp1);
  sensor2Temp.publish((int)temp2);
  sensor1Humidity.publish((int)humidity1);
  sensor2Humidity.publish((int)humidity2);


  shroom.setCurrent(temp1, temp2, humidity1, humidity2);

  fan.publish(shroom.shouldFan());
  if(shroom.shouldFan()){
    activateFan();
  } else {
    deactivateFan();
  }

  diffusor.publish(shroom.shouldDiffuse());
  if(shroom.shouldDiffuse()){
    activateHumidifier();
  } else {
    deactivateHumidifier();
  }

  //indicate to MQTT that everything is running smoothly
  reboot.publish(0); 

  //int sleepMS = Watchdog.sleep(SAMPLING_INTERVAL);  
  delay(SAMPLING_INTERVAL);

} 

void connectWiFi(void){
  WiFi.begin(WLAN_NAME, WLAN_PASSWORD);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(String(ret));
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }

  Serial.println("MQTT Connected!");
}

void setMinHumidityCallback(double humidity) {
  shroom.setMinHumidity(humidity);
  Serial.print("setting minHumidity to ");
  Serial.println(humidity);
}

void setMaxHumidityCallback(double humidity) {
  shroom.setMinHumidity(humidity);
  Serial.print("setting maxHumidity to ");
  Serial.println(humidity);
  shroom.setMaxHumidity(humidity);
}