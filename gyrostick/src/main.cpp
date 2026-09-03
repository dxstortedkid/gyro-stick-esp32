#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "modules/mpu6050/mpu6050.hpp"

LOG_MODULE_REGISTER(app, LOG_LEVEL_INF);

int main()
{
    const struct device *const dev = DEVICE_DT_GET_ANY(invensense_mpu6050);

    Mpu6050 imu(dev);

    if (!imu.init()) {
        LOG_ERR("MPU6050 initialization failed");
        return 0;
    }

    k_msleep(1000);

    imu.calibrate(200);

    LOG_INF("IMU stream started");

    ImuData data{};

    while (true) {
        if (imu.read(data)) {
            LOG_INF("ACC: [%6.2f, %6.2f, %6.2f] m/s^2 | GYR: [%6.3f, %6.3f, %6.3f] rad/s",
                    static_cast<double>(data.ax),
                    static_cast<double>(data.ay),
                    static_cast<double>(data.az),
                    static_cast<double>(data.gx),
                    static_cast<double>(data.gy),
                    static_cast<double>(data.gz));
        }

#ifndef CONFIG_MPU6050_TRIGGER
        k_msleep(20);
#endif
    }

    return 0;
}