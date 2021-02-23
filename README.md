# INA219_Power_Meter_Thingsgateway
INA219 Power Meter to Thingsgateway.

Logg every 2ms (2ms, baud 500 000, CPU 160MHz)! Mostly 2ms but sometimes 4ms.  
Logging 3ms is more stable though. 

Serial logger: 
http://www.robertovalgolio.com/sistemi-programmi/ecosystemofthings

Name output to csv instead of txt. 
Put Things gateway so that live serial output is inder screen for stability. 

Move Things gateway so that serial output (area marked 1) is "under" computer screen (increases stability)
Start logg to file (2)
Start logging serial (3)

When finished first unpress 3 and then wait for writing to file to stop (file size not increasing anymore). Then press 2. 

![](Images/Screen_Things_Gateway.JPG?raw=true)

- Wemos 3.3V -> INA219 VCC
- Wemos GND -> Battery GND, INA219 GND and Sensor GND
- Wemos D2 -> INA19 SDA
- Wemos D1-> INA9 SCL
- INA219 GND -> Battery GND and Sensor GND
- INA219 V+ -> Battery PWR
- INA219 V- -> Sensor PWR
