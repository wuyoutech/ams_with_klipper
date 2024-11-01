#include <stdbool.h>

#include "gd32f30x_it.h"
#include "gd32f30x_rcu.h"
#include "main.h"
#include "systick.h"
#include "keypanel.h"
#define I2C_TIME_OUT 100

// uint8_t BeepCnt = 0;
// uint8_t BeepDuty = 0;
// uint8_t BeepPeriod = 0;

KeyPanel_t KeyPanel;
// static uint8_t g_WdgEnRst = 0;
// static I2C_HandleTypeDef g_hi2c;

// void KeyPanel_init(void) {
//     g_hi2c.Instance = I2C2;
//     g_hi2c.Init.Timing = 0x10C0ECFF;
//     g_hi2c.Init.OwnAddress1 = 0;
//     g_hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//     g_hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//     g_hi2c.Init.OwnAddress2 = 0;
//     g_hi2c.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
//     g_hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//     g_hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

//     if (HAL_I2C_Init(&g_hi2c) != HAL_OK) {
//         Error_Handler();
//     }
//     if (HAL_I2CEx_ConfigAnalogFilter(&g_hi2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
//         Error_Handler();
//     }
//     if (HAL_I2CEx_ConfigDigitalFilter(&g_hi2c, 0) != HAL_OK) {
//         Error_Handler();
//     }
// }

// uint8_t KeyPanel_I2C_Write(uint16_t DevAddr, uint8_t *Data, uint32_t Len) {
//     uint8_t ret = 0;

//     ret = HAL_I2C_Master_Transmit(&g_hi2c, DevAddr, Data, Len, I2C_TIME_OUT);
//     if (ret != HAL_OK) {
//         HAL_I2C_DeInit(&g_hi2c);
//         HAL_I2C_Init(&g_hi2c);
//     }
//     return ret;
// }

// uint8_t KeyPanel_I2C_Read(uint16_t DevAddr, uint8_t *Data, uint32_t Len) {
//     uint8_t ret = 0;

//     ret = HAL_I2C_Master_Receive(&g_hi2c, DevAddr, Data, Len, I2C_TIME_OUT);
//     if (ret != HAL_OK) {
//         HAL_I2C_DeInit(&g_hi2c);
//         HAL_I2C_Init(&g_hi2c);
//     }
//     return ret;
// }

// void KEY_Refresh(void) {
//     static uint8_t Cnt = 0;
//     static uint8_t KeyBuf[4];

//     // 蜂鸣器刷新
//     if (BeepCnt > 0) {
//         if (++Cnt < BeepPeriod) {
//             if (Cnt < BeepDuty)
//                 BEEP_ON();
//             else
//                 BEEP_OFF();
//         } else {
//             Cnt = 0;
//             if (--BeepCnt == 0) BEEP_OFF();
//         }
//     }
//     KeyBuf[0] = 0x41;  // 使能[4:6]亮度

//     KeyPanel_I2C_Write(0x48, KeyBuf, 3);
//     KeyPanel_I2C_Write(0x68, &KeyPanel.DIG0, 1);
//     KeyPanel_I2C_Write(0x6A, &KeyPanel.DIG1, 1);
//     KeyPanel_I2C_Read(0x4F, &KeyPanel.Key, 1);
// }

// void BEEP_Set(uint8_t Period, uint8_t Duty, uint8_t Cnt) {
//     __disable_irq();
//     BeepCnt = Cnt;
//     BeepDuty = Duty;
//     BeepPeriod = Period;
//     __enable_irq();
// }

// void BEEP_Abort(void) {
//     __disable_irq();
//     BEEP_OFF();
//     BeepCnt = 0;
//     __enable_irq();
// }

// void KEY_SetPortMode(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t Mode) {
//     GPIO_InitTypeDef GPIO_InitStruct = {0};

//     GPIO_InitStruct.Pin = GPIO_Pin;
//     GPIO_InitStruct.Mode = Mode;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//     HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
// }
// /**------------------------------------------------------------------------------------------------
//  * @brief  获取键盘状态值
//  * @param  无
//  * @retval 键值
//  */
// uint16_t KEY_GetStatus(void) {
//     static uint8_t LastKey = 0;
//     static uint8_t LastVal = 0;
//     static uint8_t Count = 0;

//     if (KeyPanel.Key != LastKey) {
//         Count = 0;
//         LastKey = KeyPanel.Key;
//         if (KeyPanel.Key <= 0x40)
//             return KeyPanel.Key | 0x4000;
//         else
//             LastVal = KeyPanel.Key;
//     } else {
//         if (KeyPanel.Key <= 0x40)
//             Count = 0;
//         else
//             Count++;
//         if (Count == 2) {
//             return LastVal | 0x4000;
//         } else if (Count > 30) {
//             Count = 3;
//             return LastVal | 0x2000;
//         }
//     }
//     return 0;
// }
// /**------------------------------------------------------------------------------------------------
//  * @brief  看门狗初始化
//  * @param  无
//  * @retval 无
//  */
// void WDG_Init(void) {
//     g_WdgEnRst = 0;
//     hiwdg1.Instance = IWDG1;
//     hiwdg1.Init.Prescaler = IWDG_PRESCALER_256;
//     // hiwdg1.Init.Window = 4095;
//     hiwdg1.Init.Window = IWDG_WINDOW_DISABLE;
//     hiwdg1.Init.Reload = 13;
//     if (HAL_IWDG_Init(&hiwdg1) != HAL_OK) {
//         Error_Handler();
//     }
// }
// /**------------------------------------------------------------------------------------------------
//  * @brief  看门狗喂狗
//  * @param  无
//  * @retval 无
//  */
// void WDG_Feed(void) {
//     if (g_WdgEnRst) return;
//     HAL_IWDG_Refresh(&hiwdg1);
// }
// /**------------------------------------------------------------------------------------------------
//  * @brief  使能看门狗复位系统
//  * @param  无
//  * @retval 无
//  */
// void WDG_EnableResetSystem(void) { g_WdgEnRst = 1; }

//=================================================================================================

// keypanel i2c scl: PB6
// keypanel i2c sda: PB7
static void keypanel_i2c_init(void) {
    // enable peripheral clock
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_I2C0);

    // set to normal gpio mode to release i2c bus
    // to avoid i2c wait loop forever
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7);
    gpio_bit_set(GPIOB, GPIO_PIN_6 | GPIO_PIN_7);
    // wait a little bit time
    delay_ms(2);

    // set to i2c mode
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7);
    gpio_pin_remap_config(GPIO_I2C0_REMAP, ENABLE);

    // i2c clock configuration
    i2c_clock_config(I2C0, 100000, I2C_DTCY_2);
    i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0x48);
    i2c_enable(I2C0);
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);

    // wait until i2c bus is idle
    uint32_t wait_start = get_sys_time_ms();
    while (i2c_flag_get(I2C0, I2C_FLAG_I2CBSY)) {
        if (get_sys_time_ms() - wait_start > I2C_TIME_OUT) {
            // this should never call if i2c is correctly initialized
            Error_Handler();
        }
    }
}
static void keypanel_i2c_write(uint8_t addr, uint8_t *data, uint8_t len) {
    uint32_t wait_start;
    // wait until i2c bus is idle
    wait_start = get_sys_time_ms();
    while (i2c_flag_get(I2C0, I2C_FLAG_I2CBSY)) {
        if (get_sys_time_ms() - wait_start > I2C_TIME_OUT) {
            Error_Handler();
        }
    }
    // send a start condition to i2c bus
    i2c_start_on_bus(I2C0);
    wait_start = get_sys_time_ms();
    while (!i2c_flag_get(I2C0, I2C_FLAG_SBSEND)) {
        if (get_sys_time_ms() - wait_start > I2C_TIME_OUT) {
            Error_Handler();
        }
    }
    // send slave address to i2c bus
    i2c_master_addressing(I2C0, addr, I2C_TRANSMITTER);
    wait_start = get_sys_time_ms();
    while (!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND)) {
        if (get_sys_time_ms() - wait_start > I2C_TIME_OUT) {
            Error_Handler();
        }
    }
    // clear addsend flag
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);
    // wait until the transmit data buffer is empty
    wait_start = get_sys_time_ms();
    while (!i2c_flag_get(I2C0, I2C_FLAG_TBE)) {
        if (get_sys_time_ms() - wait_start > I2C_TIME_OUT) {
            Error_Handler();
        }
    }
    // send data to i2c bus
    for (uint8_t i = 0; i < len; i++) {
        i2c_data_transmit(I2C0, data[i]);
        wait_start = get_sys_time_ms();
        while (!i2c_flag_get(I2C0, I2C_FLAG_TBE)) {
            if (get_sys_time_ms() - wait_start > I2C_TIME_OUT) {
                Error_Handler();
            }
        }
    }
    // send a stop condition to i2c bus
    i2c_stop_on_bus(I2C0);
    wait_start = get_sys_time_ms();
    // wait until stop condition is detected
    while (I2C_CTL0(I2C0) & 0x0200) {
        if (get_sys_time_ms() - wait_start > I2C_TIME_OUT) {
            Error_Handler();
        }
    }
}
static void keypanel_i2c_read(uint8_t addr, uint8_t *data, uint8_t len) {
    uint32_t wait_start;
    // wait until i2c bus is idle
    wait_start = get_sys_time_ms();
    while (i2c_flag_get(I2C0, I2C_FLAG_I2CBSY)) {
        if (get_sys_time_ms() - wait_start > I2C_TIME_OUT) {
            Error_Handler();
        }
    }
    // send a start condition to i2c bus
    i2c_start_on_bus(I2C0);
    wait_start = get_sys_time_ms();
    while (!i2c_flag_get(I2C0, I2C_FLAG_SBSEND)) {
        if (get_sys_time_ms() - wait_start > I2C_TIME_OUT) {
            Error_Handler();
        }
    }
    // send slave address to i2c bus
    i2c_master_addressing(I2C0, addr, I2C_RECEIVER);
    wait_start = get_sys_time_ms();
    while (!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND)) {
        if (get_sys_time_ms() - wait_start > I2C_TIME_OUT) {
            Error_Handler();
        }
    }
    // clear addsend flag
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);
    // receive data from i2c bus
    for (int i = 0; i < len; i++) {
        wait_start = get_sys_time_ms();
        while (!i2c_flag_get(I2C0, I2C_FLAG_RBNE)) {
            if (get_sys_time_ms() - wait_start > I2C_TIME_OUT) {
                Error_Handler();
            }
        }
        data[i] = i2c_data_receive(I2C0);
    }
    // send a stop condition to i2c bus
    i2c_stop_on_bus(I2C0);
    wait_start = get_sys_time_ms();
    // wait until stop condition is detected
    while (I2C_CTL0(I2C0) & 0x0200) {
        if (get_sys_time_ms() - wait_start > I2C_TIME_OUT) {
            Error_Handler();
        }
    }
}

// TODO: split beeper to a separate file
void keypanel_refresh(void) {
    static uint8_t Cnt = 0;
    static uint8_t KeyBuf[4];

    // config byte:
    // [KOFF][INTENS:3][7SEG][SLEEP]0[ENA]B


    KeyBuf[0] = 0x41;  // ? maybe brightness: 010

    keypanel_i2c_write(0x48, KeyBuf, 3);          // ? set chip status, why 3 byte?
    keypanel_i2c_write(0x68, &KeyPanel.DIG0, 1);  // write digit 0
    keypanel_i2c_write(0x6A, &KeyPanel.DIG1, 1);  // write digit 1
    keypanel_i2c_read(0x4F, &KeyPanel.Key, 1);    // read key value
}

// beeper: GPIOC 2
void beep_init(void) {
    // enable peripheral clock
    rcu_periph_clock_enable(RCU_GPIOC);

    // configure gpio
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
    gpio_bit_reset(GPIOC, GPIO_PIN_2);
    gpio_bit_set(GPIOC, GPIO_PIN_2);
}
void beep_on(void) { gpio_bit_reset(GPIOC, GPIO_PIN_2); }
void beep_off(void) { gpio_bit_set(GPIOC, GPIO_PIN_2); }
