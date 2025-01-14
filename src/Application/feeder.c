#include <gd32f30x.h>

// HR8833 nSleep: PB14
// AIN1: PC6
// AIN2: PC7

// BIN1: PC9
// BIN2: PC8


void feeder_config(void) {
    // config feeder motor control pin
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);

    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_14);
    gpio_bit_set(GPIOB, GPIO_PIN_14);  // Enable motor driver

    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_6);  // motor1
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_7);

    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_8);  // motor2
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_9);

    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_8);  // motor3
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_9);

    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_10);  // motor4
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_11);
}

void feeder1_set_forward(void) {
    gpio_bit_set(GPIOC, GPIO_PIN_6);
    gpio_bit_reset(GPIOC, GPIO_PIN_7);
}
void feeder1_set_backward(void) {
    gpio_bit_reset(GPIOC, GPIO_PIN_6);
    gpio_bit_set(GPIOC, GPIO_PIN_7);
}
void feeder1_set_idle(void) {
    gpio_bit_set(GPIOC, GPIO_PIN_6);
    gpio_bit_set(GPIOC, GPIO_PIN_7);
}

void feeder2_set_forward(void) {
    gpio_bit_set(GPIOC, GPIO_PIN_8);
    gpio_bit_reset(GPIOC, GPIO_PIN_9);
}
void feeder2_set_backward(void) {
    gpio_bit_reset(GPIOC, GPIO_PIN_8);
    gpio_bit_set(GPIOC, GPIO_PIN_9);
}
void feeder2_set_idle(void) {
    gpio_bit_set(GPIOC, GPIO_PIN_8);
    gpio_bit_set(GPIOC, GPIO_PIN_9);
}

void feeder3_set_forward(void) {
    gpio_bit_set(GPIOA, GPIO_PIN_8);
    gpio_bit_reset(GPIOA, GPIO_PIN_9);
}
void feeder3_set_backward(void) {
    gpio_bit_reset(GPIOA, GPIO_PIN_8);
    gpio_bit_set(GPIOA, GPIO_PIN_9);
}
void feeder3_set_idle(void) {
    gpio_bit_set(GPIOA, GPIO_PIN_8);
    gpio_bit_set(GPIOA, GPIO_PIN_9);
}

void feeder4_set_forward(void) {
    gpio_bit_set(GPIOA, GPIO_PIN_10);
    gpio_bit_reset(GPIOA, GPIO_PIN_11);
}
void feeder4_set_backward(void) {
    gpio_bit_reset(GPIOA, GPIO_PIN_10);
    gpio_bit_set(GPIOA, GPIO_PIN_11);
}
void feeder4_set_idle(void) {
    gpio_bit_set(GPIOA, GPIO_PIN_10);
    gpio_bit_set(GPIOA, GPIO_PIN_11);
}

void mux_set_idle(void) { gpio_bit_reset(GPIOB, GPIO_PIN_14); }

enum feeder_status { feeder_status_idle, feeder_status_feed, feeder_status_reverse_to_release, feeder_status_ready };

int feeder1_status = feeder_status_idle;
void feeder1_task(void) {
    switch (feeder1_status) {
        case feeder_status_idle:
            // TODO: move adc_channel_sample(5) > 1000 to a function
            if (adc_channel_sample(5) > 1000) {
                feeder1_set_forward();
                feeder1_status = feeder_status_feed;
            }
            break;
        case feeder_status_feed:
            feeder1_set_forward();
            // TODO: identify mux module hall sensor status

            if (1) {
                feeder1_set_idle();
                feeder1_status = feeder_status_reverse_to_release;
            }
        case feeder_status_reverse_to_release:
            feeder1_set_backward();
            if (1) {
                feeder1_set_idle();
                feeder1_status = feeder_status_ready;
            }
            break;
        case feeder_status_ready:
            if (1) {
                feeder_status_idle;
            }
            break;
    }
}