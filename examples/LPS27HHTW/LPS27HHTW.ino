/**
 *  7Semi LPS27HHTW Pressure Sensor
 *
 *  Description:
 *  - Driver for ST LPS27HHTW pressure sensor
 *  - Supports I2C communication
 *  - Provides pressure (hPa) and temperature (°C)
 *
 *  Pressure Scaling:
 *  - Fixed scale → 4096 LSB/hPa
 *  - Pressure range → up to 1260 hPa
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

  // Initialize sensor (auto-detect LPS27HHTW)
  if (!baro.begin(0x5D, Wire, 400000)) {
    Serial.println("LPS27HHTW not detected!");
    while (1);
  }

  Serial.println("LPS27HHTW ready!");

  // Reset sensor
  baro.softReset();

  // NOTE:
  // LPS27HHTW does NOT support FS mode (no setScale)

  // Set Output Data Rate (ODR)
  // HZ_4 → 4 Hz update rate
  baro.setODR(LPS28DFW_ODR::HZ_4);

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

  delay(500);
}