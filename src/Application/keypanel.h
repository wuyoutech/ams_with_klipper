#pragma once

#include <stdint.h>

typedef struct {
    union {
        struct {
            uint8_t LED_Wall : 1;
            uint8_t LCD_BL : 1;
            uint8_t LED_BatGreen : 1;
            uint8_t LED_Lock : 1;
            uint8_t LED_WarnRed : 1;
            uint8_t LED_WarnGreen : 1;
            uint8_t LED_BatRed : 1;
            uint8_t UnUsed2 : 1;
        };
        uint8_t DIG0;
    };
    union {
        struct {
            uint8_t UnUsed3 : 1;
            uint8_t UnUsed4 : 1;
            uint8_t LED_2H : 1;
            uint8_t LED_10H : 1;
            uint8_t LED_8H : 1;
            uint8_t LED_6H : 1;
            uint8_t LED_4H : 1;
            uint8_t UnUsed5 : 1;
        };
        uint8_t DIG1;
    };
    uint8_t Key;
    uint8_t Cover;
    uint16_t BL_Delay;
} KeyPanel_t;

// typedef struct {
//     uint16_t Rain;
//     uint16_t LifterPos;
//     uint16_t TemperADC;
//     uint16_t StopSwitch;
//     int16_t Temperature;
// } AnalogSensorType;

// extern KeyPanel_t KeyPanel;
// /**------------------------------------------------------------------------------------------------
//  * @brief  键盘面板初始化
//  * @param  无
//  * @retval 无
//  */
// void KeyPanel_init(void);
// /**------------------------------------------------------------------------------------------------
//  * @brief  看门狗初始化
//  * @param  无
//  * @retval 无
//  */
// void WDG_Init(void);
// /**------------------------------------------------------------------------------------------------
//  * @brief  看门狗喂狗
//  * @param  无
//  * @retval 无
//  */
// void WDG_Feed(void);
// /**------------------------------------------------------------------------------------------------
//  * @brief  使能看门狗复位系统
//  * @param  无
//  * @retval 无
//  */
// void WDG_EnableResetSystem(void);
// /**------------------------------------------------------------------------------------------------
//  * @brief  蜂鸣器设置
//  * @param  Period 周期
//  * @param  Duty   占空比
//  * @param  Cnt    次数
//  * @retval 无
//  */
// void BEEP_Set(uint8_t Period, uint8_t Duty, uint8_t Cnt);
// /**------------------------------------------------------------------------------------------------
//  * @brief  蜂鸣器终止
//  * @param  无
//  * @retval 无
//  */
// void BEEP_Abort(void);
// /**------------------------------------------------------------------------------------------------
//  * @brief  按键刷新
//  * @param  无
//  * @retval 无
//  */
// void KEY_Refresh(void);
// /**------------------------------------------------------------------------------------------------
//  * @brief  获取键盘状态值
//  * @param  无
//  * @retval 键值
//  */
// uint16_t KEY_GetStatus(void);

// // #endif  // KEY_PANEL_H
// //=================================================================================================
