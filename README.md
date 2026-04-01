# 7Semi LPS28DFW Arduino Library

Arduino driver for the ST LPS28DFW and LPS27HHTW pressure sensors.

The LPS28DFW series are high-performance digital barometric pressure sensors with integrated temperature sensing. These sensors support I²C communication and are ideal for applications such as:

- Altitude measurement  
- Weather monitoring  
- Drones  
- Industrial sensing  

This library provides a clean and efficient interface for:

- Pressure (hPa) reading  
- Temperature (°C) reading  
- Advanced features like FIFO, interrupts, and calibration (offset/OPC)

---

## Features

- Pressure measurement (hPa)
- Temperature measurement (°C)
- Support for LPS28DFW and LPS27HHTW
- Automatic sensor detection
- Raw and scaled data reading
- FIFO support (buffered pressure data)
- Interrupt configuration (threshold, DRDY, FIFO)
- Pressure threshold configuration
- Low-pass filter (LPF)
- Block Data Update (BDU)
- One-shot and continuous modes
- ESP32 compatible

---

## Supported Platforms

- Arduino UNO / Mega
- ESP32
- Any board supporting Wire (I²C)

---

## Hardware

### Supported Sensors

**7Semi Pressure Sensor Breakouts:**

- LPS28DFW  
- LPS27HHTW  

---

## Connection

The sensors use **I²C communication**.

### I²C Wiring

| Sensor Pin | MCU Pin | Description |
|------------|--------|------------|
| VCC        | 3.3V   | Sensor power |
| GND        | GND    | Ground |
| SDA        | SDA    | I²C data |
| SCL        | SCL    | I²C clock |

### I²C Notes

- Default address: `0x5D`  
- Recommended speed: `100kHz – 400kHz`  

---

## Installation

### Arduino Library Manager

1. Open Arduino IDE  
2. Go to **Library Manager**  
3. Search for `7Semi LPS28DFW`  
4. Click **Install**

### Manual Installation

1. Download repository as ZIP  
2. Go to **Sketch → Include Library → Add .ZIP Library**

---

## Example

```cpp
#include <7Semi_LPS28DFW.h>

LPS28DFW_7Semi sensor;

void setup()
{
    Serial.begin(115200);

    if(!sensor.begin(0x5D))
    {
        Serial.println("Sensor not detected");
        while(1);
    }
}

void loop()
{
    float pressure, temperature;

    if(sensor.readPressure(pressure))
    {
        Serial.print("Pressure: ");
        Serial.print(pressure);
        Serial.println(" hPa");
    }

    if(sensor.readTemperature(temperature))
    {
        Serial.print("Temp: ");
        Serial.print(temperature);
        Serial.println(" °C");
    }

    delay(500);
}
```
---

## Library Overview

### Reading Pressure (hPa)

```cpp
float pressure;
sensor.readPressure(pressure);
```
Returns pressure in hPa.

### Reading Temperature (°C)

```cpp
float temp;
sensor.readTemperature(temp);
```
Returns temperature in °C.

### Reading Raw Pressure
```cpp

int32_t raw;
sensor.readPressureRaw(raw);
```
Returns raw pressure data.

## Sensor Configuration

### Output Data Rate (ODR)
```cpp
sensor.setODR(LPS28DFW_ODR::HZ_25);
```

### Averaging
```cpp
sensor.setAveraging(LPS28DFW_AVG::AVG_32);
```

### Low Pass Filter
```cpp
sensor.setLPF(true);
```
### Block Data Update (BDU)
```cpp
sensor.setBDU(true);
```

## FIFO Usage

### Enable FIFO
```cpp
sensor.setFifoConfig(true, FIFO_CONTINUOUS);
sensor.setFifoWatermark(50);
```
### Read FIFO
```cpp
float pressure;
sensor.getFifoData(pressure);
```
### Interrupts

```cpp
sensor.setInterrupt(true, false, true);
sensor.setInterruptPin(true, false, true);
```
### Pressure Threshold

```cpp
sensor.setPressureThreshold(1013.25);
```
## Example Applications

- Altitude measurement
- Weather stations
- Drones and UAVs
- Indoor navigation
- Industrial pressure monitoring
- IoT environmental sensing

## License

### MIT License

## Author

### 7Semi
