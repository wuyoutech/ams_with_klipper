#include "systick.h"
#include <gd32f30x.h>
#include <stdbool.h>

void led_on(void) { gpio_bit_write(GPIOC, GPIO_PIN_13, 0); }
void led_off(void) { gpio_bit_write(GPIOC, GPIO_PIN_13, 1); }

void led_config(void) {
    rcu_periph_clock_enable(RCU_GPIOC);
    led_off();
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
}


uint32_t led_blinking_interval = 1000;
void led_blinking_interval_set(uint32_t interval){
    led_blinking_interval = interval;
}

void led_blinking_task(void) {
    static uint32_t last_time_ms = 0;
    if (get_sys_time_ms() - last_time_ms < led_blinking_interval) {
        return;
    }
    last_time_ms = get_sys_time_ms();

    static bool led_status = 0;
    if (led_status) {
        led_on();
    } else {
        led_off();
    }
    led_status = !led_status;
}

