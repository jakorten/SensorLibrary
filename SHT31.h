#ifndef SHT31_H
#define SHT31_H

#include <stdint.h>
#include <cstdint>

using namespace std;

class SHT31 {
public:
    SHT31(const char* i2cDevice = "/dev/i2c-1", int i2cAddress = 0x44);
    ~SHT31();
    bool initialize();
    float readTemperature();
    float readHumidity();

private:
    const char* _i2cDevice;
    int _i2cAddress;
    int _i2cFile;

    const std::uint16_t SHT31_MEAS_HIGHREP = 0x2400;
    bool readTemperatureAndHumidity(float &temperature, float &humidity);
};

#endif