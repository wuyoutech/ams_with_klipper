#pragma once


#include <stdint.h>

void feeder_config(void) ;

void feeder1_set_forward(void) ;
void feeder1_set_backward(void) ;
void feeder1_set_idle(void) ;
void feeder2_set_forward(void) ;
void feeder2_set_backward(void) ;
void feeder2_set_idle(void) ;
void feeder3_set_forward(void) ;
void feeder3_set_backward(void) ;
void feeder3_set_idle(void) ;
void feeder4_set_forward(void) ;
void feeder4_set_backward(void) ;
void feeder4_set_idle(void) ;


uint16_t adc_channel_sample(uint8_t channel);
