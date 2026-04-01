#pragma once
#include "7Semi_Interface.h"

/**
 * 7Semi SPI Interface Implementation
 */
class SPI_Interface_7Semi : public Interface_7Semi {

public:
    SPIClass *spi = nullptr;
    uint8_t cs;
    uint32_t speed;

    bool beginSPI(uint8_t csPin, SPIClass &spiPort, uint32_t spiSpeed,
                  uint8_t sck, uint8_t miso, uint8_t mosi) override
    {
        spi = &spiPort;
        cs = csPin;
        speed = spiSpeed;

        pinMode(cs, OUTPUT);
        digitalWrite(cs, HIGH);

#if defined(ESP32)
        if (sck != 255 && miso != 255 && mosi != 255)
            spi->begin(sck, miso, mosi, cs);
        else
            spi->begin();
#else
        spi->begin();
#endif

        return true;
    }

    bool beginI2C(uint8_t, TwoWire &, uint32_t,
                  uint8_t, uint8_t) override
    {
        return false;
    }

    int8_t read(uint8_t reg, uint8_t *data, uint32_t len) override
    {
        spi->beginTransaction(SPISettings(speed, MSBFIRST, SPI_MODE0));

        digitalWrite(cs, LOW);
        delayMicroseconds(1);

        spi->transfer(reg | 0x80);

        for (uint32_t i = 0; i < len; i++)
            data[i] = spi->transfer(0x00);

        digitalWrite(cs, HIGH);
        spi->endTransaction();

        return 0;
    }

    int8_t write(uint8_t reg, const uint8_t *data, uint32_t len) override
    {
        spi->beginTransaction(SPISettings(speed, MSBFIRST, SPI_MODE0));

        digitalWrite(cs, LOW);

        spi->transfer(reg & 0x7F);

        for (uint32_t i = 0; i < len; i++)
            spi->transfer(data[i]);

        digitalWrite(cs, HIGH);
        spi->endTransaction();

        return 0;
    }
};