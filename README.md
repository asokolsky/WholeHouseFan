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

|Device|    |    |    | Pin| Out|    |    |    |Connector|
|------|----|----|----|----|----|----|----|----|-----|
|      |Pin1|Pin2|Pin3|Pin4|Pin5|Pin6|Pin7|Pin8|RJ-45|
|      |    |Pin1|Pin2|Pin3|Pin4|Pin5|Pin6|    |RJ-12|
|      |    |    |Pin1|Pin2|Pin3|Pin4|    |    |RJ-11|



### Fan Driver & 1Wire Hub
