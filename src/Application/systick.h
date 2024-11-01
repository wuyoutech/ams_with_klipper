#pragma once

#include <stdint.h>

/* configure systick */
void systick_config(void);
void delay_ms(uint32_t delay_time);
uint32_t get_sys_time_ms(void);
