#pragma once

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#include <stddef.h>
#include <cstddef>

struct ImuData {
    float ax, ay, az; 
    float gx, gy, gz; 
};

class Mpu6050 {
public:
    explicit Mpu6050(const struct device *dev) : m_dev(dev) {}

    bool init();

    bool calibrate(size_t samples = 200);

    bool read(ImuData &data);

private:
    const struct device *m_dev;

    ImuData m_offset{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    bool read_raw(ImuData &raw);
};