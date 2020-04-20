# ArduinoDistanceMeterMQTT

## Arduino Requirements
 - Hardware: NodeMCU v2 ESP8266
 - Hardware: HC-SR04P 3.3v
 - Breadboard, Wire, misc supplies

### Wiring Pinout
 - HC-SR04P
   - vcc > NodeMCU 3v3
   - trig > NodeMCU D2
   - echo > NodeMCU D1
   - gnd > NodeMCU GND

### Images

**Bread Board**

![Breadboard Complete](image/Breadboard-Complete.jpg)

**HC-SR04, this should be HC-SR04p as NodeMCUv2 does not support 5v sensors**

![HC-SR04](image/HC-SR04.jpg)

**NodeMCU**

![NodeMCUv2](image/NodeMCUv2.jpg)

**NodeMCU Diagram**

![NodeMCUv2Diagram](image/NodeMCUv2_Diagram.png)

**HASS Sensor Image**

![HASSSensorImage](image/HASS-Sensor-Image.PNG)
