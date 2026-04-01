#pragma once
#include <Arduino.h>
#include <Wire.h>
#include "7Semi_I2C_Interface.h"
#include "BusIO_7Semi.h"

/**
 * Default I2C address of LPS28DFW sensor
 */
#define LPS28DFW_I2C_ADDR 0x5D

#pragma once

// ---------------------------
// LPS28DFW Register Map
// ---------------------------

/**
 * INTERRUPT_CFG (0x0B)
 *
 * - Configures interrupt behavior
 * - Controls threshold and reference modes
 */
#define LPS28DFW_INTERRUPT_CFG 0x0B

/**
 * THS_P_L / THS_P_H (0x0C - 0x0D)
 *
 * - Pressure threshold value (15-bit)
 * - Used for pressure interrupt generation
 */
#define LPS28DFW_THS_P_L 0x0C
#define LPS28DFW_THS_P_H 0x0D

/**
 * IF_CTRL (0x0E)
 *
 * - Interface configuration
 * - Controls communication settings
 */
#define LPS28DFW_IF_CTRL 0x0E

/**
 * WHO_AM_I (0x0F)
 *
 * - Device identification register
 * - Expected value: 0xB4
 */
#define LPS28DFW_WHO_AM_I 0x0F

/**
 * CTRL_REG1 (0x10)
 *
 * - Output Data Rate (ODR)
 * - Averaging configuration
 */
#define LPS28DFW_CTRL_REG1 0x10

/**
 * CTRL_REG2 (0x11)
 *
 * - Reset, reboot, BDU
 * - Low-pass filter
 * - Full-scale mode
 * - Operating mode
 */
#define LPS28DFW_CTRL_REG2 0x11

/**
 * CTRL_REG3 (0x12)
 *
 * - Interrupt pin configuration
 * - Active level and output type
 */
#define LPS28DFW_CTRL_REG3 0x12

/**
 * CTRL_REG4 (0x13)
 *
 * - Interrupt routing
 * - FIFO interrupt control
 */
#define LPS28DFW_CTRL_REG4 0x13

/**
 * FIFO_CTRL (0x14)
 *
 * - FIFO mode configuration
 * - Stop-on-watermark control
 */
#define LPS28DFW_FIFO_CTRL 0x14

/**
 * FIFO_WTM (0x15)
 *
 * - FIFO watermark threshold (7-bit)
 */
#define LPS28DFW_FIFO_WTM 0x15

/**
 * REF_P_L / REF_P_H (0x16 - 0x17)
 *
 * - Reference pressure registers
 * - Used for relative pressure measurements
 */
#define LPS28DFW_REF_P_L 0x16
#define LPS28DFW_REF_P_H 0x17

/**
 * I3C_IF_CTRL (0x19)
 *
 * - I3C interface configuration
 */
#define LPS28DFW_I3C_IF_CTRL 0x19

/**
 * RPDS_L / RPDS_H (0x1A - 0x1B)
 *
 * - Pressure offset registers (16-bit)
 * - Used for one-point calibration (OPC)
 * - Format: 2's complement (signed)
 */
#define LPS28DFW_RPDS_L 0x1A
#define LPS28DFW_RPDS_H 0x1B

/**
 * INT_SOURCE (0x24)
 *
 * - Interrupt status flags
 * - Indicates threshold events and boot status
 */
#define LPS28DFW_INT_SOURCE 0x24

/**
 * FIFO_STATUS1 / FIFO_STATUS2 (0x25 - 0x26)
 *
 * - FIFO level and status flags
 * - Watermark, full, overrun indicators
 */
#define LPS28DFW_FIFO_STATUS1 0x25
#define LPS28DFW_FIFO_STATUS2 0x26

/**
 * STATUS (0x27)
 *
 * - Data ready flags
 * - Overrun indicators for pressure and temperature
 */
#define LPS28DFW_STATUS 0x27

/**
 * PRESSURE_OUT (0x28 - 0x2A)
 *
 * - 24-bit pressure output
 * - XL, L, H bytes
 */
#define LPS28DFW_PRESSURE_OUT_XL 0x28
#define LPS28DFW_PRESSURE_OUT_L 0x29
#define LPS28DFW_PRESSURE_OUT_H 0x2A

/**
 * TEMP_OUT (0x2B - 0x2C)
 *
 * - 16-bit temperature output
 */
#define LPS28DFW_TEMP_OUT_L 0x2B
#define LPS28DFW_TEMP_OUT_H 0x2C

/**
 * FIFO_DATA_OUT_PRESS (0x78 - 0x7A)
 *
 * - 24-bit pressure data from FIFO
 */
#define LPS28DFW_FIFO_DATA_OUT_PRESS_XL 0x78
#define LPS28DFW_FIFO_DATA_OUT_PRESS_L 0x79
#define LPS28DFW_FIFO_DATA_OUT_PRESS_H 0x7A

/**
 * WHO_AM_I expected value
 */
#define LPS28DFW_ID 0xB4
#define LPS27HHTW_ID 0xB3

/**
 * Supported sensor types
 */
enum class LPS2X_Device
{
    UNKNOWN = 0,
    LPS28DFW,
    LPS27HHTW
};

/**
 * Output Data Rate (ODR)
 *
 * - Controls sampling frequency
 */
enum LPS28DFW_ODR : uint8_t
{
    ONE_SHOT = 0b0000, // One-shot mode (manual trigger)
    HZ_1 = 0b0001,     // 1 Hz
    HZ_4 = 0b0010,     // 4 Hz
    HZ_10 = 0b0011,    // 10 Hz
    HZ_25 = 0b0100,    // 25 Hz
    HZ_50 = 0b0101,    // 50 Hz
    HZ_75 = 0b0110,    // 75 Hz
    HZ_100 = 0b0111,   // 100 Hz
    HZ_200 = 0b1000    // 200 Hz
};

/**
 * Averaging configuration
 *
 * - Reduces noise by averaging samples
 * - Higher value = smoother but slower response
 */
enum LPS28DFW_AVG : uint8_t
{
    AVG_4 = 0b000,
    AVG_8 = 0b001,
    AVG_16 = 0b010,
    AVG_32 = 0b011,
    AVG_64 = 0b100,
    AVG_128 = 0b101,
    AVG_512 = 0b111
};

/**
 * FIFO operating modes
 *
 * - Controls how data is stored in FIFO buffer
 */
enum LPS28DFW_FIFO_MODE
{
    FIFO_BYPASS = 0b000,                // FIFO disabled
    FIFO = 0b001,                       // Stop when full
    FIFO_CONTINUOUS = 0b010,            // Continuous overwrite
    FIFO_CONTINUOUS_ALT = 0b011,        // Alternate continuous

    FIFO_BYPASS_TO_FIFO = 0b101,        // Switch to FIFO on trigger
    FIFO_BYPASS_TO_CONTINUOUS = 0b110,  // Switch to continuous on trigger
    FIFO_CONTINUOUS_TO_FIFO = 0b111     // Continuous to FIFO transition
};
class LPS28DFW_7SEMI
{
public:
    /**
     * Constructor
     *
     * - Initializes internal pointers
     * - Ensures bus starts as null
     */
    LPS28DFW_7SEMI();

    /**
     * Destructor
     *
     * - Delete previous bus
     */
    ~LPS28DFW_7SEMI();

    /**
     * Initialize sensor with I2C interface
     *
     * - Stores device address
     * - Reinitializes bus if already allocated
     * - Configures I2C communication
     * - Allocates BusIO wrapper
     * - Verifies sensor 
     *
     * Note:
     * - Must be called before any read/write operation
     *
     * return:
     * - true  : Initialization successful
     * - false : Bus allocation failed or device not detected
     */
    bool begin(uint8_t addr = LPS28DFW_I2C_ADDR, TwoWire &wire = Wire, uint32_t speed = 400000);

    /**
     * Read product ID 
     *
     * - Reads device identification register
     * - Used to verify correct sensor connection
     *
     * Note:
     * - Call begin() before using this function
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool getProductID(uint8_t &who_am_i);

    /**
     * Enable or disable automatic reference pressure
     *
     * - Controls AUTOREFP bit in INTERRUPT_CFG register
     * - Automatically updates reference pressure value
     *
     * Note:
     * - Useful for relative pressure measurements
     *
     * return:
     * - true  : Write successful
     * - false : Bus not initialized or write failed
     */
    bool setAutoReference(bool enable);
   
    /**
     * Get automatic reference pressure status
     *
     * - Reads AUTOREFP bit from INTERRUPT_CFG register
     *
     * Note:
     * - Indicates if auto reference is enabled
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool getAutoReference(bool &enable);

    /**
     * Enable or disable auto zero function
     *
     * - Controls AUTOZERO bit in INTERRUPT_CFG register
     * - Automatically sets current pressure as reference
     *
     * Note:
     * - Useful for relative pressure measurements
     *
     * return:
     * - true  : Write successful
     * - false : Bus not initialized or write failed
     */
    bool setAutoZero(bool enable);

    /**
     * Get auto zero status
     *
     * - Reads AUTOZERO bit from INTERRUPT_CFG register
     *
     * Note:
     * - Indicates if auto zero is enabled
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool getAutoZero(bool &enable);

    /**
     * Configure pressure interrupt behavior
     *
     * - Enables high pressure interrupt (PHE)
     * - Enables low pressure interrupt (PLE)
     * - Configures interrupt latching (LIR)
     *
     * Note:
     * - Bits are written together (bit 0 to 2)
     *
     * return:
     * - true  : Write successful
     * - false : Bus not initialized or write failed
     */
    bool setInterrupt(bool enable_high_pressure, bool enable_low_pressure, bool enable_latching);
    
    /**
     * Get pressure interrupt configuration
     *
     * - Reads PHE, PLE, and LIR bits from INTERRUPT_CFG register
     *
     * Note:
     * - Returns individual interrupt enable states
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool getInterrupt(bool &enable_high_pressure, bool &enable_low_pressure, bool &enable_latching);
    
    /**
     * set pressure threshold raw value
     *
     * - Write 15-bit threshold from THS_P registers
     *
     * Note:
     * - MSB uses only lower 7 bits
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool setPressureThresholdRaw(uint16_t pressure_raw);

    /**
     * Read pressure threshold raw value
     *
     * - Reads 15-bit threshold from THS_P registers
     * - Combines MSB and LSB into single value
     *
     * Note:
     * - MSB uses only lower 7 bits
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool getPressureThresholdRaw(uint16_t &pressure_raw);

    /**
     * Set pressure threshold in hPa
     *
     * - Converts hPa value to raw register format
     * - Applies scaling based on full-scale mode
     *
     * Note:
     * - Mode1 (1260 hPa) → multiply by 16
     * - Mode2 (4060 hPa) → multiply by 8
     *
     * return:
     * - true  : Conversion and write successful
     * - false : Invalid input or write failed
     */
    bool setPressureThreshold(float hPa);

    /**
     * Get pressure threshold in hPa
     *
     * - Reads raw threshold value
     * - Converts to hPa using current FS mode
     *
     * return:
     * - true  : Read and conversion successful
     * - false : Read failed
     */
    bool getPressureThreshold(float &hPa);

    /**
     * Set Output Data Rate (ODR)
     *
     * - Configures sensor output data rate
     *
     * Note:
     * - Value must be within supported ODR range
     *
     * return:
     * - true  : Write successful
     * - false : Invalid input or bus failure
     */
    bool setODR(LPS28DFW_ODR odr); 

    /**
     * Get Output Data Rate (ODR)
     *
     * - Reads ODR bits from CTRL_REG1
     * - Get current 
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool getODR(uint8_t &odr);

    /**
     * Set internal averaging
     *
     * - Configures pressure averaging level
     * - Writes AVG bits in CTRL_REG1
     *
     * Note:
     * - Higher averaging improves noise but increases latency
     *
     * return:
     * - true  : Write successful
     * - false : Invalid input or bus failure
     */
    bool setAveraging(LPS28DFW_AVG average);
   
    /**
     * Get averaging configuration
     *
     * - Reads AVG bits from CTRL_REG1
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool getAveraging(uint8_t &average);

    /**
     * Trigger memory reboot
     *
     * - Reloads calibration parameters from memory
     * - Writes BOOT bit in CTRL_REG2
     *
     * return:
     * - true  : Write successful
     * - false : Bus not initialized or write failed
     */
    bool memoryReboot(bool reboot);

    /**
     * Configure full-scale mode
     *
     * - 0 → 1260 hPa (high resolution)
     * - 1 → 4060 hPa (wide range)
     *
     * - Writes FS_MODE bit in CTRL_REG2
     * - Updates local cache for fast access
     *
     * Note:
     * - Recommended to read back for verification
     *
     * return:
     * - true  : Write successful
     * - false : Bus not initialized or write failed
     */
    bool setScale(bool full_scale_4060);

    /**
     * Get full-scale mode
     *
     * - Reads FS_MODE bit from CTRL_REG2
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool getScale(bool &full_scale_4060);

    /**
     * Get pressure scaling factor
     *
     * - Reads FS_MODE from hardware
     * - Returns correct LSB per hPa value
     *
     * Note:
     * - FS_MODE = 0 → 4096 LSB/hPa
     * - FS_MODE = 1 → 2048 LSB/hPa
     *
     * return:
     * - Scaling factor (float)
     */
    float getPressureScale();

    /**
     * Enable or disable low-pass filter
     *
     * - Controls LPF bit in CTRL_REG2
     *
     * return:
     * - true  : Write successful
     * - false : Bus not initialized or write failed
     */
    bool setLPF(bool enable_low_pass_filter);

    /**
     * Get low-pass filter status
     *
     * - Reads LPF bit from CTRL_REG2
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool getLPF(bool &enable_low_pass_filter);

    /**
     * Enable or disable Block Data Update (BDU)
     *
     * - Prevents data overwrite until read
     * - Controls BDU bit in CTRL_REG2
     *
     * return:
     * - true  : Write successful
     * - false : Bus not initialized or write failed
     */
    bool setBDU(bool enable_update_when_read);

    /**
     * Get Block Data Update (BDU) status
     *
     * - Reads BDU bit from CTRL_REG2
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool getBDU(bool &enable_update_when_read);

    /**
     * Perform soft reset
     *
     * - Resets sensor configuration
     * - Writes SWRESET bit in CTRL_REG2
     *
     * return:
     * - true  : Write successful
     * - false : Bus not initialized or write failed
     */
    bool softReset();

    /**
     * Set operating mode
     *
     * - Enables one-shot or continuous mode
     * - Writes ONE_SHOT bit in CTRL_REG2
     *
     * Note:
     * - true  → One-shot mode
     * - false → Continuous mode
     *
     * return:
     * - true  : Write successful
     * - false : Bus not initialized or write failed
     */
    bool setOperatingMode(bool enable_one_shot_mode);

    /**
     * Get operating mode
     *
     * - Reads ONE_SHOT bit from CTRL_REG2
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool getOperatingMode(bool &enable_one_shot_mode);

    /**
     * Configure interrupt polarity and output type
     *
     * - Sets active level (active low or high)
     * - Configures output driver (push-pull or open-drain)
     *
     * Note:
     * - Writes CTRL_REG3 register
     *
     * return:
     * - true  : Write successful
     * - false : Bus not initialized or write failed
     */
    bool setInterruptConfig(bool active_low, bool open_drian);

    /**
     * Get interrupt polarity and output type
     *
     * - Reads active level and output driver configuration
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool getInterruptConfig(bool &active_low, bool &open_drian);

    /**
     * Configure interrupt pin routing
     *
     * - Controls data ready signal output
     * - Controls data ready pulse mode
     * - Controls threshold interrupt routing
     *
     * Note:
     * - Uses bits 4 to 6 in CTRL_REG4
     *
     * return:
     * - true  : Write successful
     * - false : Bus not initialized or write failed
     */
    bool setInterruptPin(bool data_ready_signal, bool data_ready_pulse, bool thershold_interreupt);
    
    /**
     * Get interrupt pin configuration
     *
     * - Reads DRDY signal, pulse, and threshold interrupt states
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool getInterruptPin(bool &data_ready_signal, bool &data_ready_pulse, bool &thershold_interreupt);

    /**
     * Configure FIFO interrupt sources
     *
     * - Enables FIFO full interrupt
     * - Enables watermark interrupt
     * - Enables FIFO overrun interrupt
     *
     * Note:
     * - Uses bits 0 to 2 in CTRL_REG4
     *
     * return:
     * - true  : Write successful
     * - false : Bus not initialized or write failed
     */
    bool setFifoIntrrupt(bool fifo_full, bool watermark_full, bool fifo_overrun);

    /**
     * Get FIFO interrupt configuration
     *
     * - Reads FIFO full, watermark, and overrun states
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool getFifoIntrrupt(bool &fifo_full, bool &watermark_full, bool &fifo_overrun);

    /**
     * Read pressure from sensor
     *
     * - Reads 24-bit pressure output registers
     * - Applies sign extension
     * - Converts using FS_MODE scaling
     *
     * Note:
     * - Output in hPa
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool readPressure(float &pressure_hPa);

    /**
     * Read temperature from sensor
     *
     * - Reads 16-bit temperature output registers
     * - Converts to degrees Celsius
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool readTemperature(float &temp_C);

    /**
     * Configure FIFO mode
     *
     * - Sets STOP_ON_WTM behavior
     * - Configures FIFO mode
     *
     * Note:
     * - Uses bits [3:0] in FIFO_CTRL
     *
     * return:
     * - true  : Write successful
     * - false : Bus not initialized or write failed
     */
    bool setFifoConfig(bool stop_on_wtm,
                       LPS28DFW_FIFO_MODE mode);

    /**
     * Get FIFO configuration
     *
     * - Reads STOP_ON_WTM and FIFO mode
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool getFifoConfig(bool &stop_on_wtm,
                       uint8_t &mode);

    /**
     * Set FIFO watermark level
     *
     * - Defines threshold level for FIFO interrupt
     *
     * Note:
     * - Valid range: 0 to 127
     *
     * return:
     * - true  : Write successful
     * - false : Invalid input or write failed
     */
    bool setFifoWatermark(uint8_t watermark);

    /**
     * Get FIFO watermark level
     *
     * - Reads watermark value from FIFO_WTM register
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool getFifoWatermark(uint8_t &watermark);

    /**
     * Read pressure from FIFO buffer
     *
     * - Reads latest FIFO sample
     * - Applies sign extension
     * - Converts using scaling
     *
     * Note:
     * - Returns false if FIFO is empty
     *
     * return:
     * - true  : Read successful
     * - false : FIFO empty or read failed
     */
    bool getFifoData(float &pressure);

    /**
     * Read reference pressure (raw)
     *
     * - Reads 16-bit reference pressure value
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool getReferencePressureRaw(uint16_t &reference_pressure);

    /**
     * Read reference pressure (hPa)
     *
     * - Converts raw reference value to hPa
     *
     * return:
     * - true  : Read successful
     * - false : Read failed
     */
    bool getReferencePressure(float &reference_pressure);

    /**
     * Write raw pressure offset (16-bit)
     *
     * - Writes offset directly to RPDS registers
     * - Format: LSB first, then MSB
     *
     * note:
     * - Wrong byte order will corrupt offset
     */
    bool setOffsetRaw(int16_t pressure_offset_raw);

    /**
     * Read raw pressure offset (16-bit)
     *
     * - Reads RPDS registers
     * - Combines LSB and MSB into 16-bit value
     *
     * note:
     * - Sensor stores data as LSB first
     */
    bool getOffsetRaw(int16_t &pressure_offset_raw);

    /**
     * Get interrupt status
     *
     * - Reads pressure threshold flags and boot status
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool getIntStatus(bool &high_pressure_threshold, bool &low_pressure_threshold, bool &boot_on);

    /**
     * Get FIFO length
     *
     * - Reads number of samples in FIFO
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool getFifoLength(uint8_t &fifo_length);

    /**
     * Get FIFO status flags
     *
     * - Reads FIFO watermark, full, and overrun flags
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool getFifoStatus(bool &fifo_watermark, bool &fifo_full, bool &fifo_overrun);

    /**
     * Get sensor status
     *
     * - Reads data ready and overrun flags
     *
     * return:
     * - true  : Read successful
     * - false : Bus not initialized or read failed
     */
    bool getSensorStatus(bool &temperature_ready, bool &pressure_ready, bool &temperature_overrun, bool &pressure_overrun);

private:
    I2C_Interface i2c;
    BusIO_7Semi<I2C_Interface> *bus;

    uint8_t fs_mode = 0;

    uint8_t address;

    LPS2X_Device device = LPS2X_Device::UNKNOWN;
};