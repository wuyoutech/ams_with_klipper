#include "main.h"

#include "feeder.h"
#include "filament_buffer.h"
#include "hall.h"
#include "led.h"
#include "systick.h"

// obrush PB12 PB13 PB8 PA6

enum feeder_status { feeder_status_idle, feeder_status_feed, feeder_status_reverse_to_release, feeder_status_ready };

struct feeder {
    uint32_t status;
    uint32_t feeder_hall_sensor;
    uint32_t mux_hall_sensor;
    void (*forward)(void);
    void (*backward)(void);
    void (*idle)(void);
};

struct feeder feeders[4] = {
    {feeder_status_idle, feeder_hall1, mux_hall1, feeder1_set_forward, feeder1_set_backward, feeder1_set_idle},
    {feeder_status_idle, feeder_hall2, mux_hall2, feeder2_set_forward, feeder2_set_backward, feeder2_set_idle},
    {feeder_status_idle, feeder_hall3, mux_hall3, feeder3_set_forward, feeder3_set_backward, feeder3_set_idle},
    {feeder_status_idle, feeder_hall4, mux_hall4, feeder4_set_forward, feeder4_set_backward, feeder4_set_idle},
};

void feeder_task(void) {
    static uint32_t last_time_ms = 0;
    if (get_sys_time_ms() - last_time_ms < 30) {
        return;
    }
    last_time_ms = get_sys_time_ms();

    for (int i = 0; i < 4; i++) {
        switch (feeders[i].status) {
        case feeder_status_idle:
            feeders[i].idle();
            if (hall_sensor_get_status(feeders[i].feeder_hall_sensor)) {
                feeders[i].status = feeder_status_feed;
            }
            break;
        case feeder_status_feed:
            feeders[i].forward();
            if (hall_sensor_get_status(feeders[i].mux_hall_sensor)) {
                feeders[i].status = feeder_status_reverse_to_release;
            }
            return;
            break;
        case feeder_status_reverse_to_release:
            feeders[i].backward();
            if (!hall_sensor_get_status(feeders[i].mux_hall_sensor)) {
                feeders[i].status = feeder_status_ready;
            }
            return;
            break;
        case feeder_status_ready:
            feeders[i].idle();
            if (!hall_sensor_get_status(feeders[i].feeder_hall_sensor)) {
                feeders[i].status = feeder_status_idle;
            }
            break;
        }
    }
}

int main(void) {
    systick_config();
    led_config();
    filament_buffer_config();
    feeder_config();
    hall_sensor_config();
    // rcu_periph_clock_enable(RCU_GPIOB);
    // rcu_periph_clock_enable(RCU_GPIOA);
    // gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_12);
    // gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_8);
    // gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_13);
    // gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_6);

    // gpio_bit_write(GPIOB, GPIO_PIN_8, 1);   // dir may be
    // gpio_bit_write(GPIOB, GPIO_PIN_12, 1);  // en
    // gpio_bit_write(GPIOB, GPIO_PIN_13, 1);
    // gpio_bit_write(GPIOA, GPIO_PIN_6, 0);  // pwm

    while (1) {
        led_blinking_task();
        feeder_task();
    }
}

// this function should never be called
// if called, it means there is a bug in the code
// or the hardware is not working correctly
// in this case, the system will be stuck in this function
// and the error led will be on

void Error_Handler(void) {
    while (1) {
    }
}
