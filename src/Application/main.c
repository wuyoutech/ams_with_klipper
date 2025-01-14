#include "main.h"

#include "feeder.h"
#include "filament_buffer.h"
#include "led.h"
#include "systick.h"

// obrush PB12 PB13 PB8 PA6

int main(void) {
    systick_config();
    led_config();
    filament_buffer_config();
    // feeder_config();
    adc_init();
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_12);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_8);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_13);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_6);

    gpio_bit_write(GPIOB, GPIO_PIN_8, 1);   // dir may be
    gpio_bit_write(GPIOB, GPIO_PIN_12, 1);  // en
    gpio_bit_write(GPIOB, GPIO_PIN_13, 1);
    gpio_bit_write(GPIOA, GPIO_PIN_6, 0);  // pwm

    while (1) {
        led_blinking_task();
        // adc_test_task();
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
