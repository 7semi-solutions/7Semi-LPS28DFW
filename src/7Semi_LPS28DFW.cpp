

#include "7Semi_LPS28DFW.h"

// ------------------------
LPS28DFW_7SEMI::LPS28DFW_7SEMI()
{
    bus = nullptr;
}

LPS28DFW_7SEMI::~LPS28DFW_7SEMI()
{
    /**
     * Destructor
     *
     * - Releases dynamically allocated bus
     * - Prevents memory leak
     */
    if (bus)
    {
        delete bus;
        bus = nullptr;
    }
}


bool LPS28DFW_7SEMI::begin(uint8_t addr, TwoWire &wire, uint32_t speed)
{
    /**
     * Store device address
     */
    address = addr;

    /**
     * Cleanup existing bus instance
     */
    if (bus)
    {
        delete bus;
        bus = nullptr;
    }

    /**
     * Initialize I2C interface
     *
     * - Sets address, wire instance, and clock speed
     */
    i2c.beginI2C(addr, wire, speed);


    //  * Create BusIO wrapper
    bus = new BusIO_7Semi<I2C_Interface>(i2c);

    if (!bus)
        return false;

    // * Confirm correct device is connected
    uint8_t id;
    if (!bus->read(LPS28DFW_WHO_AM_I, id))
        return false;

    // Detect device
    if (id == 0xB4)
        device = LPS2X_Device::LPS28DFW;
    else if (id == 0xB3)
        device = LPS2X_Device::LPS27HHTW;
    else
        return false;

    return true;
}


bool LPS28DFW_7SEMI::getProductID(uint8_t &who_am_i)
{
   
    if (!bus)
        return false;

    // Read WHO_AM_I register
    return bus->read(LPS28DFW_WHO_AM_I, &who_am_i, 1);
}

bool LPS28DFW_7SEMI::setAutoReference(bool enable)
{
   
    if (!bus)
        return false;

    // Write AUTOREFP bit (bit 7)
    return bus->writeBit(LPS28DFW_INTERRUPT_CFG, 7, enable);
}


bool LPS28DFW_7SEMI::getAutoReference(bool &enable)
{
   
    if (!bus)
        return false;

    // Read AUTOREFP bit (bit 7)
    return bus->readBit(LPS28DFW_INTERRUPT_CFG, 7, enable);
}


bool LPS28DFW_7SEMI::setAutoZero(bool enable)
{
   
    if (!bus)
        return false;

    // Write AUTOZERO bit (bit 5)
    return bus->writeBit(LPS28DFW_INTERRUPT_CFG, 5, enable);
}


bool LPS28DFW_7SEMI::getAutoZero(bool &enable)
{
   
    if (!bus)
        return false;

    // Read AUTOZERO bit (bit 5)
    return bus->readBit(LPS28DFW_INTERRUPT_CFG, 5, enable);
}



bool LPS28DFW_7SEMI::setInterrupt(bool enable_high_pressure, bool enable_low_pressure, bool enable_latching)
{
    uint8_t v = 0;

   
    if (!bus)
        return false;

    // Set PHE (bit 0)
    v |= (enable_high_pressure ? 1 : 0) << 0;

    // Set PLE (bit 1)
    v |= (enable_low_pressure ? 1 : 0) << 1;

    // Set LIR (bit 2)
    v |= (enable_latching ? 1 : 0) << 2;

    // Write 3 bits starting from bit 0
    return bus->writeBits(LPS28DFW_INTERRUPT_CFG, 0, 3, v);
}

bool LPS28DFW_7SEMI::getInterrupt(bool &enable_high_pressure, bool &enable_low_pressure, bool &enable_latching)
{
    uint8_t v = 0;

   
    if (!bus)
        return false;

    // Read INTERRUPT_CFG register
    if (!bus->read(LPS28DFW_INTERRUPT_CFG, v))
        return false;

    // Extract PHE (bit 0)
    enable_high_pressure = ((v >> 0) & 0x01) != 0;

    // Extract PLE (bit 1)
    enable_low_pressure = ((v >> 1) & 0x01) != 0;

    // Extract LIR (bit 2)
    enable_latching = ((v >> 2) & 0x01) != 0;

    return true;
}

bool LPS28DFW_7SEMI::setPressureThreshold(float hPa)
{
    // Reject invalid negative input
    if (hPa < 0)
        return false;

    // Convert hPa to raw value based on FS mode
    uint16_t raw = fs_mode ? (uint16_t)(hPa * 8)
                           : (uint16_t)(hPa * 16);

    // Write raw threshold
    return setPressureThresholdRaw(raw);
}

bool LPS28DFW_7SEMI::getPressureThreshold(float &hPa)
{
    uint16_t raw = 0;

    // Read raw threshold value
    if (!getPressureThresholdRaw(raw))
        return false;

    // Convert raw to hPa based on FS mode
    hPa = fs_mode ? (float)raw / 8.0f
                  : (float)raw / 16.0f;

    return true;
}


bool LPS28DFW_7SEMI::getPressureThresholdRaw(uint16_t &pressure_raw)
{
    uint8_t pressure_threshold[2] = {0};

   
    if (!bus)
        return false;

    // Read THS_P_L and THS_P_H registers
    if (!bus->read(LPS28DFW_THS_P_L, pressure_threshold, 2))
        return false;

    // Combine 15-bit value (MSB masked to 7 bits)
    pressure_raw = ((uint16_t)(pressure_threshold[1] & 0x7F) << 8) | pressure_threshold[0];

    return true;
}

bool LPS28DFW_7SEMI::setODR(LPS28DFW_ODR odr)
{
   
    if (!bus)
        return false;

    // Validate ODR range
    if (static_cast<uint8_t>(odr) > static_cast<uint8_t>(LPS28DFW_ODR::HZ_200))
        return false;

    // Write ODR bits (bit 3 to 6)
    return bus->writeBits(LPS28DFW_CTRL_REG1, 3, 4, static_cast<uint8_t>(odr));
}


bool LPS28DFW_7SEMI::getODR(uint8_t &odr)
{
   
    if (!bus)
        return false;

    // Read ODR bits (bit 3 to 6)
    if (!bus->readBits(LPS28DFW_CTRL_REG1, 3, 4, odr))
        return false;

    return true;
}


bool LPS28DFW_7SEMI::setAveraging(LPS28DFW_AVG average)
{
   
    if (!bus)
        return false;

    // Validate averaging range
    if (static_cast<uint8_t>(average) > static_cast<uint8_t>(LPS28DFW_AVG::AVG_512))
        return false;

    // Write AVG bits (bit 0 to 2)
    return bus->writeBits(LPS28DFW_CTRL_REG1, 0, 3, static_cast<uint8_t>(average));
}


bool LPS28DFW_7SEMI::getAveraging(uint8_t &average)
{
   
    if (!bus)
        return false;

    // Read AVG bits (bit 0 to 2)
    if (!bus->readBits(LPS28DFW_CTRL_REG1, 0, 3, average))
        return false;

    return true;
}


bool LPS28DFW_7SEMI::memoryReboot(bool reboot)
{
   
    if (!bus)
        return false;

    // Write BOOT bit (bit 7)
    return bus->writeBit(LPS28DFW_CTRL_REG2, 7, reboot);
}


bool LPS28DFW_7SEMI::setScale(bool full_scale_4060)
{
    // Not supported on LPS27HHTW
    if (device == LPS2X_Device::LPS27HHTW)
        return false;

    if (!bus)
        return false;

    if (!bus->writeBit(LPS28DFW_CTRL_REG2, 6, full_scale_4060))
        return false;

    fs_mode = full_scale_4060;

    return true;
}

bool LPS28DFW_7SEMI::getScale(bool &full_scale_4060)
{
   
    if (!bus)
        return false;

    // Read FS_MODE bit (bit 6)
    return bus->readBit(LPS28DFW_CTRL_REG2, 6, full_scale_4060);
}

float LPS28DFW_7SEMI::getPressureScale()
{
    // LPS28DFW supports dual scaling
    if (device == LPS2X_Device::LPS28DFW)
    {
        uint8_t reg = 0;

        if (!bus)
            return 4096.0f;

        if (!bus->read(LPS28DFW_CTRL_REG2, reg))
            return 4096.0f;

        bool fs = (reg >> 6) & 0x01;

        return fs ? 2048.0f : 4096.0f;
    }

    // LPS27HHTW fixed scaling
    if (device == LPS2X_Device::LPS27HHTW)
    {
        return 4096.0f;
    }

    return 4096.0f;
}

bool LPS28DFW_7SEMI::setLPF(bool enable_low_pass_filter)
{
   
    if (!bus)
        return false;

    // Write LPF bit (bit 5)
    return bus->writeBit(LPS28DFW_CTRL_REG2, 5, enable_low_pass_filter);
}

bool LPS28DFW_7SEMI::getLPF(bool &enable_low_pass_filter)
{
   
    if (!bus)
        return false;

    // Read LPF bit (bit 5)
    return bus->readBit(LPS28DFW_CTRL_REG2, 5, enable_low_pass_filter);
}

bool LPS28DFW_7SEMI::setBDU(bool enable_update_when_read)
{
   
    if (!bus)
        return false;

    // Write BDU bit (bit 3)
    return bus->writeBit(LPS28DFW_CTRL_REG2, 3, enable_update_when_read);
}


bool LPS28DFW_7SEMI::getBDU(bool &enable_update_when_read)
{
   
    if (!bus)
        return false;

    // Read BDU bit (bit 3)
    return bus->readBit(LPS28DFW_CTRL_REG2, 3, enable_update_when_read);
}

bool LPS28DFW_7SEMI::softReset()
{
   
    if (!bus)
        return false;

    // Trigger soft reset (bit 1)
    return bus->writeBit(LPS28DFW_CTRL_REG2, 1, true);
}

bool LPS28DFW_7SEMI::setOperatingMode(bool enable_one_shot_mode)
{
   
    if (!bus)
        return false;

    // Write ONE_SHOT bit (bit 0)
    return bus->writeBit(LPS28DFW_CTRL_REG2, 0, enable_one_shot_mode);
}

bool LPS28DFW_7SEMI::getOperatingMode(bool &enable_one_shot_mode)
{
   
    if (!bus)
        return false;

    // Read ONE_SHOT bit (bit 0)
    return bus->readBit(LPS28DFW_CTRL_REG2, 0, enable_one_shot_mode);
}


bool LPS28DFW_7SEMI::setInterruptConfig(bool active_low, bool open_drain)
{
    uint8_t v = 0;

   
    if (!bus)
        return false;

    // Set active level (bit 3)
    v |= (active_low ? 1 : 0) << 3;

    // Set output type (bit 1)
    v |= (open_drain ? 1 : 0) << 1;

    // Write CTRL_REG3
    return bus->write(LPS28DFW_CTRL_REG3, v);
}

bool LPS28DFW_7SEMI::getInterruptConfig(bool &active_low, bool &open_drain)
{
    uint8_t v = 0;

   
    if (!bus)
        return false;

    // Read CTRL_REG3
    if (!bus->read(LPS28DFW_CTRL_REG3, v))
        return false;

    // Extract active level (bit 3)
    active_low = (v >> 3) & 0x01;

    // Extract output type (bit 1)
    open_drain = (v >> 1) & 0x01;

    return true;
}

bool LPS28DFW_7SEMI::setInterruptPin(bool data_ready_signal,
                              bool data_ready_pulse,
                              bool threshold_interrupt)
{
    uint8_t v = 0;

   
    if (!bus)
        return false;

    // Set DRDY signal (bit 5)
    v |= (data_ready_signal ? 1 : 0) << 5;

    // Set DRDY pulse (bit 6)
    v |= (data_ready_pulse ? 1 : 0) << 6;

    // Set threshold interrupt (bit 4)
    v |= (threshold_interrupt ? 1 : 0) << 4;

    // Write bits [6:4]
    return bus->writeBits(LPS28DFW_CTRL_REG4, 4, 3, v >> 4);
}


bool LPS28DFW_7SEMI::getInterruptPin(bool &data_ready_signal,
                              bool &data_ready_pulse,
                              bool &threshold_interrupt)
{
    uint8_t v = 0;

   
    if (!bus)
        return false;

    // Read bits [6:4]
    if (!bus->readBits(LPS28DFW_CTRL_REG4, 4, 3, v))
        return false;

    // Extract DRDY signal (bit 5 → bit 1)
    data_ready_signal = (v >> 1) & 0x01;

    // Extract DRDY pulse (bit 6 → bit 2)
    data_ready_pulse = (v >> 2) & 0x01;

    // Extract threshold interrupt (bit 4 → bit 0)
    threshold_interrupt = (v >> 0) & 0x01;

    return true;
}

bool LPS28DFW_7SEMI::setFifoIntrrupt(bool fifo_full,
                               bool watermark_full,
                               bool fifo_overrun)
{
    uint8_t v = 0;

   
    if (!bus)
        return false;

    // Set FIFO full (bit 2)
    v |= (fifo_full ? 1 : 0) << 2;

    // Set watermark (bit 1)
    v |= (watermark_full ? 1 : 0) << 1;

    // Set overrun (bit 0)
    v |= (fifo_overrun ? 1 : 0) << 0;

    // Write bits [2:0]
    return bus->writeBits(LPS28DFW_CTRL_REG4, 0, 3, v);
}

bool LPS28DFW_7SEMI::getFifoIntrrupt(bool &fifo_full,
                               bool &watermark_full,
                               bool &fifo_overrun)
{
    uint8_t v = 0;

   
    if (!bus)
        return false;

    // Read CTRL_REG4
    if (!bus->read(LPS28DFW_CTRL_REG4, v))
        return false;

    // Extract FIFO full (bit 2)
    fifo_full = (v >> 2) & 0x01;

    // Extract watermark (bit 1)
    watermark_full = (v >> 1) & 0x01;

    // Extract overrun (bit 0)
    fifo_overrun = (v >> 0) & 0x01;

    return true;
}

bool LPS28DFW_7SEMI::readPressure(float &pressure_hPa)
{
    uint8_t data[3] = {0};

   
    if (!bus)
        return false;

    // Read pressure registers (3 bytes)
    if (!bus->read(LPS28DFW_PRESSURE_OUT_XL, data, 3))
        return false;

    // Combine 24-bit raw value
    int32_t raw = ((int32_t)data[2] << 16) |
                  ((int32_t)data[1] << 8) |
                  data[0];

    // Apply sign extension for 24-bit value
    if (raw & 0x800000)
        raw |= 0xFF000000;

    // Convert to hPa
    pressure_hPa = raw / getPressureScale();

    return true;
}


bool LPS28DFW_7SEMI::readTemperature(float &temp_C)
{
    uint8_t data[2] = {0};

   
    if (!bus)
        return false;

    // Read temperature registers (2 bytes)
    if (!bus->read(LPS28DFW_TEMP_OUT_L, data, 2))
        return false;

    // Combine 16-bit raw value
    int16_t raw = ((int16_t)data[1] << 8) | data[0];

    // Convert to °C
    temp_C = raw / 100.0f;

    return true;
}


bool LPS28DFW_7SEMI::setFifoConfig(bool stop_on_wtm,
                             LPS28DFW_FIFO_MODE mode)
{
    uint8_t v = 0;

   
    if (!bus)
        return false;

    // Set STOP_ON_WTM (bit 3)
    v |= (stop_on_wtm ? 1 : 0) << 3;

    // Set FIFO mode (bits [2:0])
    v |= (mode & 0x07);

    // Write FIFO_CTRL
    return bus->writeBits(LPS28DFW_FIFO_CTRL, 0, 4, v);
}


bool LPS28DFW_7SEMI::getFifoConfig(bool &stop_on_wtm,
                             uint8_t &mode)
{
    uint8_t v = 0;

   
    if (!bus)
        return false;

    // Read FIFO_CTRL
    if (!bus->read(LPS28DFW_FIFO_CTRL, v))
        return false;

    // Extract STOP_ON_WTM (bit 3)
    stop_on_wtm = (v >> 3) & 0x01;

    // Extract mode (bits [2:0])
    mode = v & 0x07;

    return true;
}

bool LPS28DFW_7SEMI::setFifoWatermark(uint8_t watermark)
{
   
    if (!bus)
        return false;

    // Validate 7-bit range
    if (watermark > 127)
        return false;

    // Write FIFO_WTM register
    return bus->write(LPS28DFW_FIFO_WTM, watermark);
}


bool LPS28DFW_7SEMI::getFifoWatermark(uint8_t &watermark)
{
   
    if (!bus)
        return false;

    // Read FIFO_WTM
    if (!bus->read(LPS28DFW_FIFO_WTM, watermark))
        return false;

    // Mask 7-bit value
    watermark &= 0x7F;

    return true;
}

bool LPS28DFW_7SEMI::getFifoData(float &pressure)
{
    uint8_t data[3];
    uint8_t length = 0;

   
    if (!bus)
        return false;

    // Check FIFO length
    if (!getFifoLength(length) || length == 0)
        return false;
    
    // Check FIFO length
    if (length == 0)
        return false;

    // Read FIFO data
    if (!bus->read(LPS28DFW_FIFO_DATA_OUT_PRESS_XL, data, 3))
        return false;

    // Combine 24-bit raw value
    int32_t raw = ((int32_t)data[2] << 16) |
                  ((int32_t)data[1] << 8) |
                  data[0];

    // Apply sign extension
    if (raw & 0x800000)
        raw |= 0xFF000000;

    // Convert to hPa
    pressure = raw / getPressureScale();

    return true;
}

bool LPS28DFW_7SEMI::getReferencePressureRaw(uint16_t &reference_pressure_raw)
{
    uint8_t data[2];

   
    if (!bus)
        return false;

    // Read REF_P registers
    if (!bus->read(LPS28DFW_REF_P_L, data, 2))
        return false;

    // Combine 16-bit value
    reference_pressure_raw = ((uint16_t)data[1] << 8) | data[0];

    return true;
}

/**
 * Read reference pressure in hPa
 *
 * - Reads raw reference pressure value
 * - Converts using current FS_MODE scaling
 *
 * note:
 * - Uses same scaling as normal pressure
 */
bool LPS28DFW_7SEMI::getReferencePressure(float &reference_pressure)
{
    uint16_t raw = 0;

    // Read raw reference value
    if (!getReferencePressureRaw(raw))
        return false;

    // Convert raw → hPa using scaling
    reference_pressure = raw / getPressureScale();

    return true;
}


bool LPS28DFW_7SEMI::setOffsetRaw(int16_t offset_raw)
{
    uint8_t data[2];

    // Check bus availability
    if (!bus)
        return false;

    // Split signed value into bytes
    data[0] = offset_raw & 0xFF;
    data[1] = (offset_raw >> 8) & 0xFF;

    return bus->write(LPS28DFW_RPDS_L, data, 2);
}

bool LPS28DFW_7SEMI::getOffsetRaw(int16_t &pressure_offset_raw)
{
    uint8_t data[2] = {0};

    if (!bus)
        return false;

    // Read 2 bytes from RPDS registers
    if (!bus->read(LPS28DFW_RPDS_L, data, 2))
        return false;

    // Combine bytes (LSB + MSB)
    pressure_offset_raw = ((int16_t)data[1] << 8) | data[0];

    return true;
}

bool LPS28DFW_7SEMI::getIntStatus(bool &high_pressure_threshold, bool &low_pressure_threshold, bool &boot_on)
{
    uint8_t v = 0;

   
    if (!bus)
        return false;

    // Read INT_SOURCE
    if (!bus->read(LPS28DFW_INT_SOURCE, v))
        return false;

    // Extract bits
    high_pressure_threshold = (v >> 0) & 0x01;
    low_pressure_threshold = (v >> 1) & 0x01;
    boot_on = (v >> 7) & 0x01;

    return true;
}


bool LPS28DFW_7SEMI::getFifoLength(uint8_t &fifo_length)
{
   
    if (!bus)
        return false;

    // Read FIFO_STATUS1
    if (!bus->read(LPS28DFW_FIFO_STATUS1, fifo_length))
        return false;

    return true;
}

bool LPS28DFW_7SEMI::getFifoStatus(bool &fifo_watermark, bool &fifo_full, bool &fifo_overrun)
{
    uint8_t v = 0;

   
    if (!bus)
        return false;

    // Read FIFO_STATUS2
    if (!bus->read(LPS28DFW_FIFO_STATUS2, v))
        return false;

    // Extract flags
    fifo_full = (v >> 0) & 0x01;
    fifo_overrun = (v >> 1) & 0x01;
    fifo_watermark = (v >> 2) & 0x01;

    return true;
}


bool LPS28DFW_7SEMI::getSensorStatus(bool &temperature_ready, bool &pressure_ready, bool &temperature_overrun, bool &pressure_overrun)
{
    uint8_t v = 0;

   
    if (!bus)
        return false;

    // Read STATUS register
    if (!bus->read(LPS28DFW_STATUS, v))
        return false;

    // Extract flags
    temperature_ready = (v >> 1) & 0x01;
    pressure_ready = (v >> 0) & 0x01;
    temperature_overrun = (v >> 5) & 0x01;
    pressure_overrun = (v >> 4) & 0x01;

    return true;
}