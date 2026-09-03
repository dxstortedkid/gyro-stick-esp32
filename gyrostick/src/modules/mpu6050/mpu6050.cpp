#include "mpu6050.hpp"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(mpu6050, LOG_LEVEL_INF);

static constexpr float GRAVITY_MSS = 9.80665f;

#ifdef CONFIG_MPU6050_TRIGGER
static K_SEM_DEFINE(data_sem, 0, 1);

static void trigger_handler(const struct device *dev,
                            const struct sensor_trigger *trig)
{
    ARG_UNUSED(dev);
    ARG_UNUSED(trig);
    k_sem_give(&data_sem);
}
#endif

bool Mpu6050::init()
{
    if (!device_is_ready(m_dev)) {
        LOG_ERR("Device %s not ready", m_dev->name);
        return false;
    }

#ifdef CONFIG_MPU6050_TRIGGER
    const struct sensor_trigger trig = {
        .type = SENSOR_TRIG_DATA_READY,
        .chan = SENSOR_CHAN_ALL,
    };

    if (sensor_trigger_set(m_dev, &trig, trigger_handler) < 0) {
        LOG_ERR("Failed to set trigger");
        return false;
    }
    LOG_INF("MPU6050: interrupt mode");
#else
    LOG_INF("MPU6050: polling mode");
#endif

    return true;
}

bool Mpu6050::read_raw(ImuData &raw)
{
#ifdef CONFIG_MPU6050_TRIGGER
    if (k_sem_take(&data_sem, K_MSEC(50)) != 0) {
        return false;
    }
#endif

    if (sensor_sample_fetch(m_dev) < 0) {
        return false;
    }

    struct sensor_value accel[3];
    struct sensor_value gyro[3];

    if (sensor_channel_get(m_dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0 ||
        sensor_channel_get(m_dev, SENSOR_CHAN_GYRO_XYZ, gyro) < 0) {
        return false;
    }

    raw.ax = sensor_value_to_float(&accel[0]);
    raw.ay = sensor_value_to_float(&accel[1]);
    raw.az = sensor_value_to_float(&accel[2]);

    raw.gx = sensor_value_to_float(&gyro[0]);
    raw.gy = sensor_value_to_float(&gyro[1]);
    raw.gz = sensor_value_to_float(&gyro[2]);

    return true;
}

bool Mpu6050::calibrate(size_t samples)
{
    LOG_INF("Calibrating MPU6050 (%u samples)... Keep motionless!", samples);

    float sum_ax = 0.0f, sum_ay = 0.0f, sum_az = 0.0f;
    float sum_gx = 0.0f, sum_gy = 0.0f, sum_gz = 0.0f;

    size_t valid = 0;
    while (valid < samples) {
        ImuData temp{};
        if (read_raw(temp)) {
            sum_ax += temp.ax;
            sum_ay += temp.ay;
            sum_az += temp.az;

            sum_gx += temp.gx;
            sum_gy += temp.gy;
            sum_gz += temp.gz;
            valid++;
        }

#ifndef CONFIG_MPU6050_TRIGGER
        k_msleep(5);
#endif
    }

    const float inv = 1.0f / static_cast<float>(samples);

    m_offset.gx = sum_gx * inv;
    m_offset.gy = sum_gy * inv;
    m_offset.gz = sum_gz * inv;

    m_offset.ax = sum_ax * inv;
    m_offset.ay = sum_ay * inv;
    m_offset.az = (sum_az * inv) - GRAVITY_MSS;

    LOG_INF("Offsets -> GYR [rad/s]: [%.4f, %.4f, %.4f] | ACC [m/s²]: [%.2f, %.2f, %.2f]",
            static_cast<double>(m_offset.gx),
            static_cast<double>(m_offset.gy),
            static_cast<double>(m_offset.gz),
            static_cast<double>(m_offset.ax),
            static_cast<double>(m_offset.ay),
            static_cast<double>(m_offset.az));

    return true;
}

bool Mpu6050::read(ImuData &data)
{
    if (!read_raw(data)) {
        return false;
    }

    data.ax -= m_offset.ax;
    data.ay -= m_offset.ay;
    data.az -= m_offset.az;

    data.gx -= m_offset.gx;
    data.gy -= m_offset.gy;
    data.gz -= m_offset.gz;

    return true;
}