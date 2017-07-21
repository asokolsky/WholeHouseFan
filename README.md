# Whole House Fan

This is an Arduino controller for a whole house fan (for definition see https://en.wikipedia.org/wiki/Whole-house_fan) controlled manually or on the basis of multiple temperature sensors.
Controller hopefully has GUI.
Maybe Internet connection.
Datalogging possible but not now.


## System Architecture

Here is the plan!
```
+----------------------+   +------------+   +-----+
| 24V DC Power Supply  |==>| Fan Driver |==>| Fan |
+----------------------+   |            |   +-----+
     +-----------------+   |     &      |   +------------------------------+
     | 1Wire devices   |==>|            |<=>| MCU with display with keypad |
     +-----------------+|  |  1Wire Hub |==>|                              |
      +-----------------+  +------------+   +------------------------------+
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

If I decide to stick with a 3.3V MCU (higly likely) I will supply 3.3V to the sensors instead of 5V.
I will need temerature sensors for:
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
The simplest solution would be to just use a single MOSFET to drive the fan.  But no.  I am getting a VNH5019 Motor Driver
https://www.pololu.com/product/1451

### Connection Between Fan Driver With Hub and MCU
I think I will use a single CAT-6 cable with RJ-45 connector.

|Device|   |   |   |Pin |Out |   |   |   |Connector|
|------|---|---|---|----|----|---|---|---|-----|
|      |1  |2  |3  |4   |5   |6  |7  |8  |RJ-45|
|      |INA|INB|PWM|+24V|+24V|DQ |GND|GND|     |

Now the Driver supplies to Controller with power (note POE-compatible pin-out!) to be stepped down to +5V or +3.3V.
Controller supplies to the Driver INA, INB, PWM signals.
Controller supplies to the Hub DQ.
Driver supplies to the Hub 24V to be stepped down to +5V or +3.3V.


### MCU with display and a keypad
I am thinking to start with something as simple as Nano. 
Just read temperatures.  Display those on a small OLED display.
Offer manual control over the fan.

## Hardware

Details at https://easyeda.com/asokolsky/Whole_House_Fan-096c6e4a6d674583abff0211818fedb9

### Schematics
![Schematics](https://github.com/asokolsky/WholeHouseFan/doc/schematic.png "schematic")

### PCB
![PCB Top](https://github.com/asokolsky/WholeHouseFan/doc/pcb-0.1.top.png "PCB Top")
![PCB Bottom](https://github.com/asokolsky/WholeHouseFan/doc/pcb-0.1.bot.png "PCB Bottom")


## Software
For schematics and PCB check out 


## Libraries

### DS18B20
Use one of
https://github.com/milesburton/Arduino-Temperature-Control-Library
or
https://github.com/matmunk/DS18B20
or
https://github.com/nettigo/DS18B20






