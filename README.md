# ShroomGroom

The ShroomGroom is an automated mushroom growing system that enables monitoring of humidity and temperature and controls ventilation and air humidification. It is build with arduino.

## MQTT communication 
It sends Messages to an MQTT brooker on the following channels as integers:

| Channel                            | Value                                     | Description                                              |
|------------------------------------|-------------------------------------------|----------------------------------------------------------|
| /shroomgroom/sensor1temp           | temperature of sensor 1 in °C             |                                                          |
| /shroomgroom/sensor2temp           | temperature of sensor 2 in °C             |                                                          |
| /shroomgroom/sensor1humidity       | humidity of sensor 1 in %                 |                                                          |
| /shroomgroom/sensor2humidity       | humidity of sensor 2 in %                 |                                                          |
| /shroomgroom/fan                   | calculated state of the fan (1 or 0)      | monitors the state of the pin that controls the fan      |
| /shroomgroom/diffusor              | calculated state of the diffusor (1 or 0) | monitors the state of the pin that controls the diffusor |
| /shroomgroom/configuredMinHumidity | lower humidity bound                      | when at least one sensor is below, turns on diffusor     |
| /shroomgroom/configuredMaxHumidity | upper humidity bound                      | when at least one sensor is below, turns on fan          |
| /shroomgroom/reboot                | 1 after restart, 0 when running           | |

You can send messages on the following channels to configure the ShroomGroom:

| Channel                     | Value                      | Description                   |
|-----------------------------|----------------------------|-------------------------------|
| /shroomgroom/setMinHumidity | configure the min humidity | send an int between 0 and 100 |
| /shroomgroom/setMaxHumidity | configure the max humidity | send an int between 0 and 100 |

There is no Logic preventing minHumidity to be higher than maxHumidity yet.



