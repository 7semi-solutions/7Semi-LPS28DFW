/**
 *  7Semi LPS28DFW Pressure baro
 *
 *  Description:
 *  - Driver for ST LPS28DFW pressure baro
 *  - Supports I2C communication
 *  - Provides pressure (hPa) and temperature (°C)
 *
 *  Pressure Scaling:
 *  - FS_MODE = 0 → 4096 LSB/hPa (1260 hPa range)
 *  - FS_MODE = 1 → 2048 LSB/hPa (4060 hPa range)
 *
 *  CONNECTION 
 *
 *  baro Pin   →   MCU (Arduino)
 *  --------------------------------
 *  VCC          →   3.3V
 *  GND          →   GND
 *  SDA          →   SDA (A4 / GPIO21)
 *  SCL          →   SCL (A5 / GPIO22)
 *
 *  I2C Address:
 *  - 0x5D (default)
 */
#include <7Semi_LPS28DFW.h>


LPS28DFW_7SEMI baro;

void setup() {
  Serial.begin(115200);
  delay(200); 

  // Initialize sensor (I2C address, Wire, speed)
  if (!baro.begin(0x5D, Wire, 400000)) {
    Serial.println("LPS28DFW not detected!");
    while (1)
      ;  // Stop execution
  }

  Serial.println("LPS28DFW ready!");

  // Reset sensor 
  baro.softReset();

  // Set full-scale mode
  // true  → 4060 hPa (wide range)
  // false → 1260 hPa (higher resolution)
  baro.setScale(true);

  // Set Output Data Rate (ODR)
  // HZ_4 → 4 Hz update rate
  baro.setODR(HZ_4);

  Serial.println("Configuration complete\n");
}

void loop() {
  float pressure = 0.0f;
  float temperature = 0.0f;

  // Read pressure (hPa)
  if (baro.readPressure(pressure)) {
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" hPa");
  } else {
    Serial.println("Pressure read failed");
  }

  // Read temperature (°C)
  if (baro.readTemperature(temperature)) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
  } else {
    Serial.println("Temperature read failed");
  }

  Serial.println("-----------------------");

  delay(500);  // Delay between readings
}