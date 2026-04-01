#pragma once
#include <Arduino.h>


template <typename Bus>
class BusIO_7Semi {

public:

    /**
     * Constructor
     *
     * - Stores interface reference
     */
    BusIO_7Semi(Bus &busRef) : bus(busRef) {}

    /** read 8-bit */
    inline bool read(uint8_t reg, uint8_t &value)
    {
        return (bus.read(reg, &value, 1) == 0);
    }

    /** read burst */
    inline bool read(uint8_t reg, uint8_t *data, uint32_t len)
    {
        return (bus.read(reg, data, len) == 0);
    }

    /** write 8-bit */
    inline bool write(uint8_t reg, uint8_t value)
    {
        return (bus.write(reg, &value, 1) == 0);
    }

    /** write burst */
    inline bool write(uint8_t reg, const uint8_t *data, uint32_t len)
    {
        return (bus.write(reg, data, len) == 0);
    }

    /** write bits */
    bool readBits(uint8_t reg, uint8_t pos, uint8_t len, uint8_t &value) {
        uint8_t reg_val;
        if (!read(reg, reg_val)) return false;
        uint8_t mask = (1 << len) - 1;
        value = (reg_val >> pos) & mask;
        return true;
    }

    bool writeBits(uint8_t reg, uint8_t pos, uint8_t len, uint8_t value) {
        uint8_t reg_val;
        if (!read(reg, reg_val)) return false;
        uint8_t mask = (1 << len) - 1;
        reg_val &= ~(mask << pos);
        reg_val |= (value & mask) << pos;
        return write(reg, reg_val);
    }

    bool readBit(uint8_t reg, uint8_t pos, bool &value) {
        uint8_t tmp;
        if (!readBits(reg, pos, 1, tmp)) return false;
        value = tmp;
        return true;
    }

    bool writeBit(uint8_t reg, uint8_t pos, bool value) {
        return writeBits(reg, pos, 1, value ? 1 : 0);
    }
private:
    Bus &bus;
};