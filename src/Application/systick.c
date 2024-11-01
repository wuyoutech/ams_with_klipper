#include "systick.h"

#include "gd32f30x.h"

void systick_config(void) {
    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 1000U)) {
        /* capture error */
        while (1) {
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}

volatile uint32_t time_ms = 0U;

void SysTick_Handler(void) { time_ms += 1U; }

void delay_ms(uint32_t delay_time) {
    uint32_t start_time = time_ms;
    while (time_ms - start_time < delay_time) {
    }
}

uint32_t get_sys_time_ms(void) { return time_ms; }


