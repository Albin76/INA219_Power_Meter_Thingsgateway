# INA219_Power_Meter_Thingsgateway
INA219 Power Meter to Thingsgateway.

Logg every 2ms (2ms, baud 500 000, CPU 160MHz)! Mostly 2ms but sometimes 4ms.  
logging 3ms is more stable though. 

Serial logger: 
http://www.robertovalgolio.com/sistemi-programmi/ecosystemofthings

Name output to csv instead of txt. 
Put Things gateway so that live serial output is inder screen for stability. 

Move Things gateway so that serial output (area marked 1) is "under" computer screen (increases stability)
Start logg to file (2)
Start logging serial (3)

When finished first unpress 3 and then wait for writing to file to stop (file size not increasing anymore). Then press 2. 

![](Images/Screen_Things_Gateway.JPG?raw=true)
