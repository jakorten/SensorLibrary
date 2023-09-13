#include "SHT31.h"
#include <iostream>
#include <unistd.h>
#include <stdint.h>
#include <cstdint>

int main() {
    SHT31 sensor;
    if (!sensor.initialize()) {
        std::cerr << "Initialization failed!" << std::endl;
        return -1;
    }

    while (true) {
        std::cout << "Temperature: " << sensor.readTemperature() << " °C, Humidity: " << sensor.readHumidity() << "%" << std::endl;
        sleep(1);
    }

    return 0;
}
