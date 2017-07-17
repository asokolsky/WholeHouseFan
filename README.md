# Whole House Fan

This is an Arduino controller for a whole house fan driven manually or on the basis of multiple temperature sensors.  
Hopefully has GUI.
Maybe Internet connection.
Datalogging possible but not now.


## System Architecture

Here is the plan!
```
+----------------------+   +------------+   +-----+
| 24V DC Power Supply  |==>| Fan Driver |==>| Fan |
+----------------------+   |    &       |   +-----+
     +-----------------+   |  1Wire Hub |   +------------------------------+
     | 1Wire devices   |==>|            |<=>| MCU with display with keypad |
     +-----------------+|  +------------+   +------------------------------+
      +-----------------+
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
I think I will use two CAT6 cables:

|Device|   |   |   |Pin|Out|   |   |   |Connector|
|------|---|---|---|---|---|---|---|---|-----|
|      |1  |2  |3  |4  |5  |6  |7  |8  |RJ-45|
|Hub   |   |   |GND|DQ |GND|+5V|   |   |     |
|Driver|INA|INB|GND|PWM|GND|VDD|+5V|+5V|     |

Controller to supply tot he Hub +5V (or +3.3V) and to read DQ.
Driver to supply to the Controller +5V.
Controller to supply to the driver VDD (+3.3V), PWM, INA, INB.



### MCU with display with keypad


