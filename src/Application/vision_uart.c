#include <gd32f30x.h>
#include <stdbool.h>
#include <string.h>

uint8_t usart_rx_buffer[256];
uint8_t usart_rx_buffer_dma[256];

uint8_t usart_tx_buffer[256];
uint8_t usart_tx_buffer_dma[256];

void wifi_dataprocess(uint8_t* data, uint32_t len) {
    uint16_t crc;
    uint8_t* Dat = data;
    uint16_t Length = len;

    if (Dat[0] != 0x55 || Dat[1] != 0xAA) goto ERR;
    if (CRC_16(Dat, Length) != 0) goto ERR;

    // 返回版本信息
    if (Dat[2] == 0x04 && Dat[3] == 'V') {
        if (Dat[4] == 'M' || Dat[4] == 'I')
            memcpy(&usart_tx_buffer[5], Version, 8);
        else if (Dat[4] == 'P')
            memcpy(&usart_tx_buffer[5], Power.Version, 8);
        else if (Dat[4] == 'C')
            memcpy(&usart_tx_buffer[5], &MotorCtrl.Cut.Version, 8);
        else if (Dat[4] == 'D')
            memcpy(&usart_tx_buffer[5], &MotorCtrl.Left.Version, 8);
        else if (Dat[4] == 'd')
            memcpy(&usart_tx_buffer[5], &MotorCtrl.Right.Version, 8);
        else
            goto ERR;

        WifiTxBuf[0] = 0x55;
        WifiTxBuf[1] = 0xAA;
        WifiTxBuf[2] = 0x04;
        WifiTxBuf[3] = 'V';
        WifiTxBuf[4] = Dat[4];
        crc = CRC_16(WifiTxBuf, 13);
        WifiTxBuf[13] = crc;
        WifiTxBuf[14] = crc >> 8;
        WifiTxBufLen = 15;
        HAL_UART_Transmit_DMA(&WIFI_AT, WifiTxBuf, WifiTxBufLen);
        return;
    }
    // 模式切换
    else if (Dat[2] == 0x03) {
        if (Dat[3] == 'A')  // 进入应用程序
        {
            if (Dat[4] == 'M' || Dat[4] == 'I') {
                WifiTxBuf[0] = 0x55;
                WifiTxBuf[1] = 0xAA;
                WifiTxBuf[2] = 0x03;
                WifiTxBuf[3] = 'A';
                WifiTxBuf[4] = Dat[4];
                crc = CRC_16(WifiTxBuf, 5);
                WifiTxBuf[5] = crc;
                WifiTxBuf[6] = crc >> 8;
                WifiTxBufLen = 7;
                HAL_UART_Transmit_DMA(&WIFI_AT, WifiTxBuf, WifiTxBufLen);
                return;
            }
        } else if (Dat[3] == 'R')  // 进入引导程序
        {
            WifiTxBuf[0] = 0x55;
            WifiTxBuf[1] = 0xAA;
            WifiTxBuf[2] = 0x03;
            WifiTxBuf[3] = 'R';
            WifiTxBuf[4] = 'M';
            crc = CRC_16(WifiTxBuf, 5);
            WifiTxBuf[5] = crc;
            WifiTxBuf[6] = crc >> 8;
            WifiTxBufLen = 7;
            HAL_UART_Transmit_DMA(&WIFI_AT, WifiTxBuf, WifiTxBufLen);
            // 复位标记
            HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR30, 'R');
            HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR31, 'T');
            // 复位系统
            WDG_EnableResetSystem();
            return;
        }
    }
    // 时间同步
    else if (Dat[2] == 0x06 && Dat[3] == 'S') {
        RTC_TimeTypeDef Time = {0};
        RTC_DateTypeDef Date = {0};

        Date.Year = Dat[4];
        Date.Month = Dat[5];
        Date.Date = Dat[6];
        Date.WeekDay = Dat[7];
        Time.Hours = Dat[8];
        Time.Minutes = Dat[9];
        Time.Seconds = Dat[10];
        Time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
        Time.StoreOperation = RTC_STOREOPERATION_SET;
        HAL_RTC_SetTime(&hrtc, &Time, RTC_FORMAT_BIN);
        HAL_RTC_SetDate(&hrtc, &Date, RTC_FORMAT_BIN);

        WifiTxBuf[0] = 0x55;
        WifiTxBuf[1] = 0xAA;
        WifiTxBuf[2] = 0x06;
        WifiTxBuf[3] = 'S';
        crc = CRC_16(WifiTxBuf, 4);
        WifiTxBuf[4] = crc;
        WifiTxBuf[5] = crc >> 8;
        WifiTxBufLen = 6;
        HAL_UART_Transmit_DMA(&WIFI_AT, WifiTxBuf, WifiTxBufLen);
        return;
    }
    // 导航指令
    else if (Dat[2] == 0x20 && Dat[3] == 'G') {
        MotorCtrl.ManualLeftSpeed = Dat[5] << 8 | Dat[4];
        MotorCtrl.ManualRightSpeed = Dat[7] << 8 | Dat[6];
        MotorCtrl.ManualCutSpeed = Dat[9] << 8 | Dat[8];

        // 回调函数
        if (WIFI_ReceivedCallback != NULL) WIFI_ReceivedCallback(&Dat[10], 10);

        WifiTxBuf[0] = 0x55;
        WifiTxBuf[1] = 0xAA;
        WifiTxBuf[2] = 0x20;
        WifiTxBuf[3] = 'G';
        // 系统时间
        RTC_TimeTypeDef Time = {0};
        RTC_DateTypeDef Date = {0};
        HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN);
        HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN);
        // 日期时间
        WifiTxBuf[4] = Date.Year;
        WifiTxBuf[5] = Date.Month;
        WifiTxBuf[6] = Date.Date;
        WifiTxBuf[7] = Time.Hours;
        WifiTxBuf[8] = Time.Minutes;
        WifiTxBuf[9] = Time.Seconds;
        // 电机数据
        WifiTxBuf[10] = MotorCtrl.Left.Status.All;
        WifiTxBuf[11] = MotorCtrl.Left.Status.All >> 8;
        WifiTxBuf[12] = MotorCtrl.Left.Speed;
        WifiTxBuf[13] = MotorCtrl.Left.Speed >> 8;
        WifiTxBuf[14] = MotorCtrl.Left.Position;
        WifiTxBuf[15] = MotorCtrl.Left.Position >> 8;

        WifiTxBuf[16] = MotorCtrl.Right.Status.All;
        WifiTxBuf[17] = MotorCtrl.Right.Status.All >> 8;
        WifiTxBuf[18] = MotorCtrl.Right.Speed;
        WifiTxBuf[19] = MotorCtrl.Right.Speed >> 8;
        WifiTxBuf[20] = MotorCtrl.Right.Position;
        WifiTxBuf[21] = MotorCtrl.Right.Position >> 8;

        WifiTxBuf[22] = MotorCtrl.Cut.Status.All;
        WifiTxBuf[23] = MotorCtrl.Cut.Status.All >> 8;
        WifiTxBuf[24] = MotorCtrl.Cut.Speed;
        WifiTxBuf[25] = MotorCtrl.Cut.Speed >> 8;

        // IMU数据
        memcpy(&WifiTxBuf[26], &IMU_Data.Pitch, 12);
        // GPS数据
        memcpy(&WifiTxBuf[38], &GPS_Info.Longitude, 16);
        // 寄存器数据
        memcpy(&WifiTxBuf[54], &Param, 12);

        crc = CRC_16(WifiTxBuf, 66);
        WifiTxBuf[66] = crc;
        WifiTxBuf[67] = crc >> 8;
        WifiTxBufLen = 68;

        HAL_UART_Transmit_DMA(&WIFI_AT, WifiTxBuf, WifiTxBufLen);
        return;
    }
ERR:
    WifiTxBuf[0] = 0x55;
    WifiTxBuf[1] = 0xAA;
    WifiTxBuf[2] = 0xEE;
    WifiTxBuf[3] = 'E';
    crc = CRC_16(WifiTxBuf, 4);
    WifiTxBuf[4] = crc;
    WifiTxBuf[5] = crc >> 8;
    WifiTxBufLen = 6;
    HAL_UART_Transmit_DMA(&WIFI_AT, WifiTxBuf, WifiTxBufLen);
}

void vision_uart_init(void) {
    // enable peripheral clock
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_UART3);
    rcu_periph_clock_enable(RCU_DMA1);
    rcu_periph_clock_enable(RCU_AF);

    // set usart tx and rx pin
    gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

    // configure dma
    // !DMA not work, use interrupt instead
    // DMA channel: DMA1 channel 2: UART3_RX  DMA1 channel 4: UART3_TX
    // DMA channel is hard coded, cannot be changed
    dma_deinit(DMA1, DMA_CH2);
    dma_parameter_struct dma_init_struct;
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)usart_rx_buffer_dma;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = 256;
    dma_init_struct.periph_addr = (uint32_t) & (USART_DATA(UART3));
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA1, DMA_CH2, &dma_init_struct);

    // set usart peripheral
    usart_deinit(UART3);
    usart_baudrate_set(UART3, 115200U);
    usart_receive_config(UART3, USART_RECEIVE_ENABLE);
    usart_transmit_config(UART3, USART_TRANSMIT_ENABLE);
    usart_dma_receive_config(UART3, USART_RECEIVE_DMA_ENABLE);
    usart_enable(UART3);

    // enable usart interrupt
    nvic_irq_enable(UART3_IRQn, 0, 0);
    usart_flag_clear(UART3, USART_FLAG_IDLE);
    usart_interrupt_enable(UART3, USART_INT_IDLE);
    usart_flag_clear(UART3, USART_FLAG_RBNE);
    usart_interrupt_enable(UART3, USART_INT_RBNE);
}

volatile bool receive_flag = 0;
volatile uint8_t rx_count = 0;
// this funtion should be called in main loop
void vision_uart_process(void) {
    if (!receive_flag) {
        return;
    }
    // call the old process function

    // clear receive flag
    receive_flag = 0;
}

void UART3_IRQHandler(void) {
    static uint8_t rx_count_interrupt = 0;

    if (usart_interrupt_flag_get(UART3, USART_INT_FLAG_RBNE)) {
        usart_interrupt_flag_clear(UART3, USART_INT_FLAG_RBNE);
        usart_rx_buffer_dma[rx_count_interrupt++] = usart_data_receive(UART3);
    }
    if (usart_interrupt_flag_get(UART3, USART_INT_FLAG_IDLE)) {
        // usart_interrupt_flag_clear(UART3, USART_INT_FLAG_IDLE);
        usart_data_receive(UART3);
        // copy data to normal buffer
        memcpy(usart_rx_buffer, usart_rx_buffer_dma, rx_count_interrupt);
        // notify main loop
        receive_flag = 1;
        rx_count = rx_count_interrupt;
        rx_count_interrupt = 0;
    }
}