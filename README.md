# Whole House Fan

This is an ESP8266 (WeMos D1 Mini) controller for a whole house fan - 
for definition see https://en.wikipedia.org/wiki/Whole-house_fan 
The fan is controlled manually or via MQTT broker.


## System Architecture

Here is the plan!
```
+----------------------+   +------------+   +-----+
| 24V DC Power Supply  |==>| Fan Driver |==>| Fan |
+----------------------+   |  1Wire Hub |   +-----+
     +-----------------+   |            |
     | 1Wire devices   |==>|            |
     +-----------------+|  |  MCU       |
      +-----------------+  +------------+
```

### Power Supply
For now, this is a switching AC-to-DC power supply.  24V chosen to reduce losses due to high current.
15A is an overkill but a prudent one given tendency for eBay vendors to overcommit.
Possible alternative: an acid or LiIon or whatever battery, maybe even solar powered.

### 1Wire Devices
I am thinking temperature sensors, e.g. DS18B20.  Possibilities: humidity, current.
Midon Design 1-Wire Application Guide  1.04 is available from 
http://www.midondesign.com/Documents/1-Wire%20Application%20Guidev104.pdf

Sensors will be terminated into RJ-11 or RJ-12 or RJ-45 connectors with the following pin-out:

|Device|  |  |  |Pin|Out|   |  |  |Connector|
|------|--|--|--|---|---|---|--|--|-----|
|      |1 |2 |3 |4  |5  |6  |7 |8 |RJ-45|
|      |  |1 |2 |3  |4  |5  |6 |  |RJ-12|
|      |  |  |1 |2  |3  |4  |  |  |RJ-11|
|Sensor|  | |GND|DQ |GND|+5V|  |  ||

If I decide to stick with a 3.3V MCU (likely) I will supply 3.3V to the sensors instead of 5V.
I will need temperature sensors for:
  * Network closet (in the housing with MCU/Display/Keypad)
  * Attic
  * North balcony
  * South balcony


### Fan
Main ctiteria for the choice was low noise, efficiency and ease of installation between joists.  I chose Spal 30102566 
http://www.spal-usa.com/fans/automated/highperformance_14_curved.html
The next step in reducing the noise would be suspending the fan just like in QuietCool products:
http://www.wholehousefandeals.com/qc-es-2250.html
The latter site can be also used for information on sizing the fan to the volume of your house.

### Fan Driver & 1Wire Hub
The simplest solution would be to use a single logical level MOSFET to drive the fan. And this is what I plan!

## Hardware

Details at https://easyeda.com/asokolsky/New_Project-a64426de9312427c8bccbf24d4158ea6

### Schematics
![Schematics](https://raw.githubusercontent.com/asokolsky/WholeHouseFan/master/doc/schematic.png "schematic")

### PCB
![PCB Top](https://raw.githubusercontent.com/asokolsky/WholeHouseFan/master/doc/pcb-0.1.top.png "PCB Top")
![PCB Bottom](https://raw.githubusercontent.com/asokolsky/WholeHouseFan/master/doc/pcb-0.1.bot.png "PCB Bottom")


## Software
Work in progress.

## Libraries

### DS18B20
Use one of
https://github.com/milesburton/Arduino-Temperature-Control-Library
or
https://github.com/matmunk/DS18B20
or
https://github.com/nettigo/DS18B20






