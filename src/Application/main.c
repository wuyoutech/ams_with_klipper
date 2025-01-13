#include "main.h"

#include "feeder.h"
#include "filament_buffer.h"
#include "led.h"
#include "systick.h"

#include <gd32f30x.h>

struct adc_sample_value {
    uint16_t feeder_hall1;
    uint16_t feeder_hall2;
    uint16_t feeder_hall3;
    uint16_t feeder_hall4;

    uint16_t mux_hall1;
    uint16_t mux_hall2;
    uint16_t mux_hall3;
    uint16_t mux_hall4;
};

struct adc_sample_value adc_value;

// void adc_test_task(void) {
//     static uint32_t last_time_ms = 0;
//     if (get_sys_time_ms() - last_time_ms < 50) {
//         return;
//     }
//     last_time_ms = get_sys_time_ms();

//     // uint16_t channel1 = adc_channel_sample(5);
//     // uint16_t channel2 = adc_channel_sample(7);
//     // uint16_t channel3 = adc_channel_sample(14);
//     // uint16_t channel4 = adc_channel_sample(15);
//     uint16_t channel1 = adc_channel_sample(10);
//     uint16_t channel2 = adc_channel_sample(11);
//     uint16_t channel3 = adc_channel_sample(12);
//     uint16_t channel4 = adc_channel_sample(13);
// }

void adc_init(void) {
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_AF);

    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_5);
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_7);
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_4);
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_5);

    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_0);
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_1);
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_2);
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_3);

    // dma init
    rcu_periph_clock_enable(RCU_DMA0);
    dma_deinit(DMA0, DMA_CH0);
    dma_parameter_struct dma_data_parameter;
    dma_data_parameter.periph_addr = (uint32_t)(&ADC_RDATA(ADC0));
    dma_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_data_parameter.memory_addr = (uint32_t)(&adc_value);
    dma_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
    dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_16BIT;
    dma_data_parameter.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_data_parameter.number = 8;
    dma_data_parameter.priority = DMA_PRIORITY_HIGH;
    dma_init(DMA0, DMA_CH0, &dma_data_parameter);
    dma_circulation_enable(DMA0, DMA_CH0);
    dma_channel_enable(DMA0, DMA_CH0);

    rcu_periph_clock_enable(RCU_ADC0);
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV8);

    adc_mode_config(ADC_MODE_FREE);
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);
    adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);

    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 8);

    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_5, ADC_SAMPLETIME_55POINT5);
    adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_7, ADC_SAMPLETIME_55POINT5);
    adc_regular_channel_config(ADC0, 2, ADC_CHANNEL_14, ADC_SAMPLETIME_55POINT5);
    adc_regular_channel_config(ADC0, 3, ADC_CHANNEL_15, ADC_SAMPLETIME_55POINT5);
    adc_regular_channel_config(ADC0, 4, ADC_CHANNEL_10, ADC_SAMPLETIME_55POINT5);
    adc_regular_channel_config(ADC0, 5, ADC_CHANNEL_11, ADC_SAMPLETIME_55POINT5);
    adc_regular_channel_config(ADC0, 6, ADC_CHANNEL_12, ADC_SAMPLETIME_55POINT5);
    adc_regular_channel_config(ADC0, 7, ADC_CHANNEL_13, ADC_SAMPLETIME_55POINT5);

    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);

    adc_dma_mode_enable(ADC0);
    adc_enable(ADC0);
    adc_calibration_enable(ADC0);

    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
}

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
    gpio_bit_write(GPIOB, GPIO_PIN_12, 1);// en
    gpio_bit_write(GPIOB, GPIO_PIN_13, 1);
    gpio_bit_write(GPIOA, GPIO_PIN_6, 0);

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
