#include <gd32f30x.h>

// feeder1 sensor: PA5

void feeder1_config(void) {
    // config ADC for feeder
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_ADC0);

    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);

    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_5);

    adc_mode_config(ADC_MODE_FREE);

    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1U);

    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);

    adc_enable(ADC0);

    adc_calibration_enable(ADC0);
}

uint16_t adc_channel_sample(uint8_t channel){
    adc_regular_channel_config(ADC0,0U,ADC_CHANNEL_5,ADC_SAMPLETIME_71POINT5);
    adc_software_trigger_enable(ADC0,ADC_REGULAR_CHANNEL);

    while(!adc_flag_get(ADC0,ADC_FLAG_EOC));
    adc_flag_clear(ADC0,ADC_FLAG_EOC);

    return adc_regular_data_read(ADC0);
}