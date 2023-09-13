#include "SHT31.h"
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <iostream>
#include <math.h>

SHT31::SHT31(const char* i2cDevice, int i2cAddress) : _i2cDevice(i2cDevice), _i2cAddress(i2cAddress), _i2cFile(-1) {}

SHT31::~SHT31() {
    if (_i2cFile != -1) {
        close(_i2cFile);
    }
}

bool SHT31::initialize() {
    _i2cFile = open(_i2cDevice, O_RDWR);
    if (_i2cFile < 0) {
        std::cerr << "Failed to open the I2C bus." << std::endl;
        return false;
    }
    if (ioctl(_i2cFile, I2C_SLAVE, _i2cAddress) < 0) {
        std::cerr << "Failed to acquire bus access or talk to slave." << std::endl;
        return false;
    }
    return true;
}

float SHT31::readTemperature() {
    float temperature, humidity;
    if (readTemperatureAndHumidity(temperature, humidity)) {
        return temperature;
    } else {
        return std::NAN;
    }
}

float SHT31::readHumidity() {
    float temperature, humidity;
    if (readTemperatureAndHumidity(temperature, humidity)) {
        return humidity;
    } else {
        return std::NAN;
    }
}

bool SHT31::readTemperatureAndHumidity(float &temperature, float &humidity) {
    std::uint8_t command[2];
    command[0] = SHT31_MEAS_HIGHREP >> 8;
    command[1] = SHT31_MEAS_HIGHREP & 0xFF;
    
    if (write(_i2cFile, command, 2) != 2) {
        std::cerr << "Failed to write to the sensor." << std::endl;
        return false;
    }

    usleep(15000);  // Measurement delay

    std::uint8_t data[6];
    if (read(_i2cFile, data, 6) != 6) {
        std::cerr << "Failed to read from the sensor." << std::endl;
        return false;
    }

    std::uint16_t temp_raw = (data[0] << 8) | data[1];
    std::uint16_t humidity_raw = (data[3] << 8) | data[4];

    temperature = 175.0f * temp_raw / 65535.0f - 45.0f;
    humidity = 100.0f * humidity_raw / 65535.0f;

    return true;
}
