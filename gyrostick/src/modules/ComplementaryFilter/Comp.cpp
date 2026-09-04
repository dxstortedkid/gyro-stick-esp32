#Етот файл нужен для того что бы накидать логику фильтра
#Использовать тип данных float первоночально потом переделать на uint_16
#Добится перевода данных с дачтика в градусы

/* пример main читай по подключению readme
Тупой пример чтения данных из реализованного класаа датчика

#include <zephyr/kernel.h>
#include "modules/mpu6050/mpu6050.hpp"

int main()
{
    const struct device *const dev = DEVICE_DT_GET_ANY(invensense_mpu6050);

    Mpu6050 imu(dev); сделать статикой

    k_msleep(1000);

    imu.calibrate(200);

    ImuData data{};

    while (true) {
        if (imu.read(data)) {
            LOG_INF("ACC: [%6.2f, %6.2f, %6.2f] m/s^2 | GYR: [%6.3f, %6.3f, %6.3f] rad/s",
                    static_cast<double>(data.ax),
                    static_cast<double>(data.ay),
                    static_cast<double>(data.az),
                    static_cast<double>(data.gx),
                    static_cast<double>(data.gy),
                    static_cast<double>(data.gz)); либо стандартные зефр convert_to_float
        }

    }

    return 0;
}
*/

/* overlay на успешный запуск i2c
&pinctrl {
	i2c0_default: i2c0_default {
		group1 {
			pinmux = <I2C0_SDA_GPIO8>, на нормальные gpio переделать
				    <I2C0_SCL_GPIO9>; https://github.com/zephyrproject-rtos/zephyr/blob/main/dts/riscv/espressif/esp32c3/esp32c3_common.dtsi?plain=1#L225
			bias-pull-up;
			drive-open-drain;
			output-high;
		};
	};

*/