#include <gd32f30x.h>

// PA2: sensor input
// PA2: TIMER1_CH2
void filament_buffer_config(void) {
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_AF);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_2);

    nvic_irq_enable(TIMER1_IRQn, 1, 1);
    nvic_irq_enable(TIMER4_IRQn, 1, 1);

    rcu_periph_clock_enable(RCU_TIMER1);
    timer_deinit(TIMER1);

    timer_ic_parameter_struct timer_icinitpara;
    timer_parameter_struct timer_initpara;

    timer_initpara.prescaler = 19;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = 65535;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1, &timer_initpara);

    timer_icinitpara.icpolarity = TIMER_IC_POLARITY_RISING;
    timer_icinitpara.icselection = TIMER_IC_SELECTION_DIRECTTI;
    timer_input_capture_config(TIMER1, TIMER_CH_2, &timer_icinitpara);

    timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_CH2);
    timer_interrupt_enable(TIMER1, TIMER_INT_CH2);
    timer_enable(TIMER1);

    // config timer4 for falling edge
    rcu_periph_clock_enable(RCU_TIMER4);
    timer_deinit(TIMER4);

    timer_init(TIMER4, &timer_initpara);

    timer_icinitpara.icpolarity = TIMER_IC_POLARITY_FALLING;
    timer_input_capture_config(TIMER4, TIMER_CH_2, &timer_icinitpara);
    timer_interrupt_flag_clear(TIMER4, TIMER_INT_FLAG_CH2);
    timer_interrupt_enable(TIMER4, TIMER_INT_CH2);
    timer_enable(TIMER4);
}

uint32_t ic1value = 0, ic2value = 0;
__IO uint16_t dutycycle = 0;
__IO uint16_t frequency = 0;

void TIMER1_IRQHandler(void) {
    if (timer_interrupt_flag_get(TIMER1, TIMER_INT_FLAG_CH2)) {
        timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_CH2);

        ic1value = timer_channel_capture_value_register_read(TIMER1, TIMER_CH_2) + 1;
        ic2value = timer_counter_read(TIMER4) + 1;
        timer_counter_value_config(TIMER1, 0);

        dutycycle = ic2value * 1000 / ic1value;
    }
}

void TIMER4_IRQHandler(void) {
    if (timer_interrupt_flag_get(TIMER4, TIMER_INT_FLAG_CH2)) {
        timer_interrupt_flag_clear(TIMER4, TIMER_INT_FLAG_CH2);

        timer_counter_value_config(TIMER4, 0);
    }
}

int filament_buffer_get(void) { return dutycycle; }
