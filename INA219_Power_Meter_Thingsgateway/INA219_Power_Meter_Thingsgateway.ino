/*
   Used in conjunction with Things Gateway: 
   http://www.robertovalgolio.com/sistemi-programmi/ecosystemofthings

   Old logger logged with Adafruit to SD-card. Approx 6-7ms between samples for 29 of them then one with 12-13ms due to buffer. 
   This loggs new ina library and to csv to Things Gateway (way faster): 760us for measurement. But vat this rate the recieving program hangs sometime and some samples are linger. At 3ms it is fully stable.
   If lowering the program so that the live serial output is outside screen it is more stable. 

   If logging with micros instead:
   Serial.println(micros()-currentMillis);  // May not be inlcuded for ThingsGateway

   Tested setups: 
   Lolin D1 mini  80MHz Serial 115200 Wire 100k: 1671 us to measure (1860 incl serial) (Measured with serialprint in code so not reliable)
   Lolin D1 mini 160MHz Serial 500000 Wire 400k:  421 us to measure (140 per measurement) (760 incl serial). 
   Lolin32 lite  240MHz Serial 115200 Wire 100k: 1820 us to measure (Measured with serialprint in code so not reliable) Why slower than D1 mini 80MHz?  

   Triggered instead of continuous takes longer to measure! I do not get duplicate readings with continuous do not get same reading twice.
   
*/

#include <Wire.h>
#include <INA219_WE.h>
#define I2C_ADDRESS 0x40

INA219_WE ina219(I2C_ADDRESS);

unsigned long currentMillis =0;
unsigned long previousMillis = 0;
unsigned long interval = 600;  // Under 3 the recieving program crashes anyway.

// Timer values
unsigned long timemillis[5];

float shuntVoltage_mV = 0.0;
float loadVoltage_V = 0.0;
float busVoltage_V = 0.0;
float current_mA = 0.0;
//float power_mW = 0.0;   // Not using
//bool ina219_overflow = false; // Not using

void ina219values() {
  //Serial.println(micros()-currentMillis);  // May not be inlcuded for ThingsGateway. Only for test of timing.
  //ina219.startSingleMeasurement(); // triggers single-shot measurement and waits until completed
  //Serial.println(micros()-currentMillis);  // May not be inlcuded for ThingsGateway. Only for test of timing.
  // timemillis[0] = micros();
  shuntVoltage_mV = ina219.getShuntVoltage_mV();
  // timemillis[1] = micros();
  busVoltage_V = ina219.getBusVoltage_V();
  // timemillis[2] = micros();
  current_mA = ina219.getCurrent_mA();
  // timemillis[3] = micros();
  //power_mW = ina219.getBusPower();
  loadVoltage_V  = busVoltage_V + (shuntVoltage_mV/1000);
  // timemillis[4] = micros();

  /*
  Serial.print("Shunt Voltage [mV]: "); Serial.println(shuntVoltage_mV);
  Serial.print("Bus Voltage [V]: "); Serial.println(busVoltage_V);
  Serial.print("Load Voltage [V]: "); Serial.println(loadVoltage_V);
  Serial.print("Current[mA]: "); Serial.println(current_mA);
  Serial.print("Bus Power [mW]: "); Serial.println(power_mW);
  */

/*
  ina219_overflow = ina219.getOverflow(); 
  
  if(!ina219_overflow){
    Serial.println("Values OK - no overflow");
  }
  else{
    Serial.println("Overflow! Choose higher PGAIN");
  }
*/  
  //Serial.println(micros()-currentMillis);  // May not be inlcuded for ThingsGateway. Only for test of timing.
}

void setup() {
  //Serial.begin(115200);
  Serial.begin(500000);
  Wire.setClock(400000); // This reduced time considerably 1547us to 421us
  Wire.begin();  // Wemos D1 mini
  //Wire.begin(19,23);  // Lolin32 Lite
  if(!ina219.init()){
    Serial.println("INA219 not connected!");
  }

  /* Set ADC Mode for Bus and ShuntVoltage
  * Mode *            * Res / Samples *       * Conversion Time *
  BIT_MODE_9        9 Bit Resolution             84 µs
  BIT_MODE_10       10 Bit Resolution            148 µs  
  BIT_MODE_11       11 Bit Resolution            276 µs
  BIT_MODE_12       12 Bit Resolution            532 µs  (DEFAULT)
  SAMPLE_MODE_2     Mean Value 2 samples         1,06 ms
  SAMPLE_MODE_4     Mean Value 4 samples         2,13 ms
  SAMPLE_MODE_8     Mean Value 8 samples         4,26 ms
  SAMPLE_MODE_16    Mean Value 16 samples        8,51 ms     
  SAMPLE_MODE_32    Mean Value 32 samples        17,02 ms
  SAMPLE_MODE_64    Mean Value 64 samples        34,05 ms
  SAMPLE_MODE_128   Mean Value 128 samples       68,10 ms
  */
  ina219.setADCMode(BIT_MODE_9); // choose mode and uncomment for change of default
  
  /* Set measure mode
  POWER_DOWN - INA219 switched off
  TRIGGERED  - measurement on demand
  ADC_OFF    - Analog/Digital Converter switched off
  CONTINUOUS  - Continuous measurements (DEFAULT)
  */
  ina219.setMeasureMode(CONTINUOUS); // Choose mode and uncomment for change of default
  //ina219.setMeasureMode(TRIGGERED); // Triggered measurements for this example
  
  /* Set PGain
  * Gain *  * Shunt Voltage Range *   * Max Current *
   PG_40       40 mV                    0,4 A
   PG_80       80 mV                    0,8 A
   PG_160      160 mV                   1,6 A
   PG_320      320 mV                   3,2 A (DEFAULT)
  */
  ina219.setPGain(PG_40); // choose gain and uncomment for change of default
  
  /* Set Bus Voltage Range
   BRNG_16   -> 16 V
   BRNG_32   -> 32 V (DEFAULT)
  */
  ina219.setBusRange(BRNG_16); // choose range and uncomment for change of default

  Serial.println("INA219 Current Sensor Example Sketch - Continuous");

  /* If the current values delivered by the INA219 differ by a constant factor
     from values obtained with calibrated equipment you can define a correction factor.
     Correction factor = current delivered from calibrated equipment / current delivered by INA219
  */
  // ina219.setCorrectionFactor(0.98); // insert your correction factor if necessary
  
}

void loop() {
  //currentMillis = millis(); // use in actual program
  currentMillis = micros(); // to use when timing
  if (currentMillis - previousMillis >= interval)
  {
   previousMillis = currentMillis;

   ina219values();
/*
   Serial.printf("Millis0: %lu\n", timemillis[0]);
   Serial.printf("Millis1: %lu\n", timemillis[1]);
   Serial.printf("Millis2: %lu\n", timemillis[2]);
   Serial.printf("Millis3: %lu\n", timemillis[3]);      
   Serial.printf("Millis4: %lu\n", timemillis[4]);
*/
   //Serial.println(micros()-currentMillis);  // May not be inlcuded for ThingsGateway. Only for test of timing.
   Serial.print("INI,");
   //Serial.print(outputCount);
   Serial.print(1);  //outputCount
   Serial.print(",");
   Serial.print(interval);
   Serial.print("\n");
   Serial.print("LOG,");
   Serial.print(currentMillis);
   Serial.print(",");
   Serial.print(loadVoltage_V);    
   Serial.print(",");
   Serial.print(current_mA);       
   Serial.print("\n");
   Serial.print("END");
   Serial.print("\n");


   //Serial.println(micros()-currentMillis);  // May not be inlcuded for ThingsGateway. Only for test of timing.
   //Serial.print("INI," + String(1) + "," + String(interval) + "\nLOG," + String(currentMillis) + "," + String(loadVoltage_V) + "," + String(current_mA)+ "\nEND\n");  // Slower
   //Serial.printf("INI,1,%i\nLOG,%u,%f,%f\nEND\n",interval,currentMillis,loadVoltage_V,current_mA); // Slowest
   //Serial.println();  // May not be inlcuded for ThingsGateway. Only for test of timing.   
   //Serial.println();  // May not be inlcuded for ThingsGateway. Only for test of timing.  
   //delay(3000);    // For testing purpose only. Not in final.
   }
   delayMicroseconds(10); 
}
