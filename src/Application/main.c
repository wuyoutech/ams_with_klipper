#include "main.h"

#include "feeder.h"
#include "filament_buffer.h"
#include "led.h"
#include "systick.h"

void adc_test_task(void) {
    static uint32_t last_time_ms = 0;
    if (get_sys_time_ms() - last_time_ms < 50) {
        return;
    }
    last_time_ms = get_sys_time_ms();

    uint16_t channel1 = adc_channel_sample(5);
    uint16_t channel2 = adc_channel_sample(7);
    uint16_t channel3 = adc_channel_sample(14);
    uint16_t channel4 = adc_channel_sample(15);
}

int main(void) {
    systick_config();
    led_config();
    filament_buffer_config();
    feeder_config();

    while (1) {
        led_blinking_task();
        adc_test_task();
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
