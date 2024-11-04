#include "main.h"

#include "vision_uart.h"

int main(void) {
    // Note: clock initialized in SystemInit(), 8M External Crystal and 120M CPU clock
    systick_config();
    vision_uart_init();
    while (1) {
        vision_uart_process();
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
