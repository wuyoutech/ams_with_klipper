#include <gd32f30x.h>

// HR8833 nSleep: PB14
// AIN1: PC6
// AIN2: PC7

// BIN1: PC9
// BIN2: PC8

// main hall: PC0 PC1 PC2 PC3 // may be



void feeder_config(void) {
    // config adc for filament sensor
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_ADC0);
    // config adc clock
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);
    // config adc pin
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_5);
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_7);
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_4);
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_5);

    // pc 0123 for hall sensor
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_0);
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_1);
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_2);
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_3); 

    // config adc mode
    adc_mode_config(ADC_MODE_FREE);
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1U);
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
    adc_enable(ADC0);
    adc_calibration_enable(ADC0);

    // config feeder motor control pin
    rcu_periph_clock_enable(RCU_GPIOB);
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
    gpio_bit_reset(GPIOC, GPIO_PIN_6);
    gpio_bit_reset(GPIOC, GPIO_PIN_7);
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
    gpio_bit_reset(GPIOC, GPIO_PIN_8);
    gpio_bit_reset(GPIOC, GPIO_PIN_9);
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
    gpio_bit_reset(GPIOA, GPIO_PIN_8);
    gpio_bit_reset(GPIOA, GPIO_PIN_9);
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
    gpio_bit_reset(GPIOA, GPIO_PIN_10);
    gpio_bit_reset(GPIOA, GPIO_PIN_11);
}

uint16_t adc_channel_sample(uint8_t channel) {
    adc_regular_channel_config(ADC0, 0U, channel, ADC_SAMPLETIME_71POINT5);
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);

    while (!adc_flag_get(ADC0, ADC_FLAG_EOC));
    adc_flag_clear(ADC0, ADC_FLAG_EOC);

    return adc_regular_data_read(ADC0);
}

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