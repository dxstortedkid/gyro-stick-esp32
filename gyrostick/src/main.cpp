#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>

static constexpr pwm_dt_spec in3 = PWM_DT_SPEC_GET(DT_NODELABEL(motor_b_in3));
static constexpr pwm_dt_spec in4 = PWM_DT_SPEC_GET(DT_NODELABEL(motor_b_in4));

int main()
{
    while (true) {
        // 50% мощности вперед (25 000 нс из периода 50 000 нс)
        pwm_set_pulse_dt(&in3, 25000);
        pwm_set_pulse_dt(&in4, 0);
        k_msleep(2000);

        // Стоп (выбег)
        pwm_set_pulse_dt(&in3, 0);
        pwm_set_pulse_dt(&in4, 0);
        k_msleep(2000);
    }

    return 0;
}