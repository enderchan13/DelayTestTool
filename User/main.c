/* Copyright 2025 Ender Chan (Ender)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Header Files */
#include "usb_host_config.h"
#include "tim.h"
#include "lcd.h"
#include "key.h"

#define DELAY_TEST_CNT 100
#define WAVE_TRIGGER_DAC_VALUE 2000
#define WAVE_RELEASE_DAC_VALUE 3500
#define SINE_WAVE_POINTS 1000

static volatile uint32_t waveIndex = 0;

uint8_t  product_name[20];          /* 设备名称 */
uint8_t  dev_sta = 0;               /* 设备状态 */
uint32_t press_delay = 9999;        /* 按下延时 */
uint32_t lift_delay  = 9999;        /* 抬起延时 */
uint8_t  test_mode = 0;             /* 测试模式 */
uint8_t  test_sta = 0;              /* 测试状态 */
uint8_t  test_cnt = DELAY_TEST_CNT; /* 测试次数 */
uint8_t  test_cnt_mode = 0;         /* 测试次数模式 */
uint16_t key_trigger_cnt = 0;       /* 一次测试的多键判断计数 */
uint16_t multi_key_Judg_cnt = 0;    /* 一次按下/抬起动作的多键判断计数 */

uint8_t  square_wave_flag = 0;              /* 方波开启标志 */
uint16_t square_wave_cnt = 0;               /* 方波周期计数 */
uint32_t square_wave_time = 0;              /* 方波计时器 */
uint8_t  square_single_press_flag = 0;      /* 方波按下标志 */
uint8_t  square_single_release_flag = 0;    /* 方波抬起标志 */
uint32_t square_single_trigger_cnt  = 0;    /* 方波按下计数器 */
uint32_t square_single_release_cnt  = 0;    /* 方波抬起计数器 */
uint16_t square_trigger_delay_time_buf[DELAY_TEST_CNT] = {0};   /* 方波按下延时数据缓存 */
uint16_t square_release_delay_time_buf[DELAY_TEST_CNT] = {0};   /* 方波抬起延时数据缓存 */ 
uint32_t square_trigger_delay_time = 0;     /* 方波按下平均延时 */
uint32_t square_release_delay_time = 0;     /* 方波抬起平均延时 */
uint8_t  square_data_invalid = 0;           /* 方波数据有效标志 */
uint8_t  square_trigger_sta = 0;            /* 方波高低电平状态 */

uint8_t  triangle_wave_flag = 0;            /* 三角波开启标志 */
uint32_t triangle_wave_cnt = 0;             /* 三角波周期计数 */
uint32_t triangle_wave_time = 0;            /* 三角波计时器 */
uint8_t  triangle_wave_press_flag = 0;      /* 三角波按下标志 */
uint8_t  triangle_wave_release_flag = 0;    /* 三角波抬起标志 */
uint32_t triangle_single_trigger_cnt = 0;   /* 三角波按下计数器 */
uint32_t triangle_single_release_cnt = 0;   /* 三角波抬起计数器 */
uint16_t triangle_trigger_delay_time_buf[DELAY_TEST_CNT] = {0};     /* 三角波按下延时数据缓存 */
uint16_t triangle_release_delay_time_buf[DELAY_TEST_CNT] = {0};     /* 三角波抬起延时数据缓存 */
uint32_t triangle_trigger_delay_time = 0;   /* 三角波按下平均延时 */
uint32_t triangle_release_delay_time = 0;   /* 三角波抬起平均延时 */
uint8_t  triangle_data_invalid = 0;         /* 三角波数据有效标志 */

uint8_t  simulation_curve_flag = 0;         /* 仿真点击开启标志 */
uint32_t simulation_curve_cnt = 0;          /* 仿真点击周期计数 */
uint32_t simulation_curve_time = 0;         /* 仿真点击计时器 */
uint8_t  simulation_curve_press_flag = 0;   /* 仿真点击按下标志 */
uint8_t  simulation_curve_release_flag = 0; /* 仿真点击抬起标志 */
uint32_t simulation_curve_single_trigger_cnt = 0;   /* 仿真点击按下计数器 */
uint32_t simulation_curve_single_release_cnt = 0;   /* 仿真点击抬起计数器 */
uint16_t simulation_curve_trigger_delay_time_buf[DELAY_TEST_CNT] = {0};     /* 仿真点击按下延时数据缓存 */
uint16_t simulation_curve_release_delay_time_buf[DELAY_TEST_CNT] = {0};     /* 仿真点击抬起延时数据缓存 */
uint32_t simulation_curve_trigger_delay_time = 0;   /* 仿真点击按下平均延时 */
uint32_t simulation_curve_release_delay_time = 0;   /* 仿真点击抬起平均延时 */
uint8_t  simulation_curve_data_invalid = 0;         /* 仿真点击数据有效标志 */
uint32_t simulation_curve_timeout = 0;              /* 仿真点击周期超时计数器 */

static uint16_t sineWaveTable[SINE_WAVE_POINTS] = {
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,

    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000,
    2000, 2001, 2002, 2005, 2007, 2010, 2012, 2016, 2019, 2021,
    2025, 2028, 2031, 2036, 2041, 2045, 2050, 2056, 2061, 2066,
    2072, 2077, 2083, 2089, 2096, 2102, 2107, 2114, 2119, 2125,
    2132, 2139, 2147, 2155, 2164, 2172, 2180, 2190, 2199, 2210,
    2222, 2234, 2247, 2260, 2274, 2290, 2305, 2321, 2337, 2354,
    2373, 2391, 2410, 2429, 2449, 2470, 2492, 2513, 2535, 2557,
    2580, 2603, 2626, 2649, 2672, 2695, 2718, 2742, 2765, 2789,
    2812, 2836, 2860, 2883, 2906, 2929, 2951, 2973, 2994, 3015,
    3037, 3057, 3077, 3098, 3118, 3136, 3156, 3173, 3191, 3208,
    3226, 3243, 3259, 3275, 3291, 3306, 3320, 3334, 3348, 3361,
    3373, 3385, 3396, 3408, 3419, 3430, 3442, 3453, 3462, 3471,
    3481, 3490, 3495, 3498, 3498, 3499, 3500, 3500, 3500, 3500,

    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,

    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3496, 3490, 
    3483, 3475, 3466, 3454, 3440, 3423, 3404, 3382, 3357, 3329, 
    3297, 3263, 3224, 3180, 3134, 3086, 3033, 2977, 2918, 2856, 
    2789, 2718, 2645, 2568, 2486, 2399, 2309, 2212, 2110, 2000,
};

/*********************************************************************
 * @fn      Dac_Init
 *
 * @brief   Initializes DAC collection.
 *
 * @return  none
 */
static void Dac_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    DAC_InitTypeDef  DAC_InitType = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_0);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_4);

    DAC_InitType.DAC_Trigger = DAC_Trigger_None;
    DAC_InitType.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
    DAC_InitType.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(DAC_Channel_1, &DAC_InitType);
    DAC_Cmd(DAC_Channel_1, ENABLE);

    DAC_SetChannel1Data(DAC_Align_12b_R, 0);
}

static void dac_simulation_curve_generate(void)
{
    static uint8_t simulation_curve_flag_temp = 0;
    if (simulation_curve_flag_temp != simulation_curve_flag) {
        simulation_curve_flag_temp = simulation_curve_flag;
        if (simulation_curve_flag) {
            multi_key_Judg_cnt = 0;
            simulation_curve_cnt = 0;
            simulation_curve_time = get_tim4_10us();
            simulation_curve_data_invalid = 0;
            test_sta = 1;
            simulation_curve_single_trigger_cnt = 0;
            simulation_curve_single_release_cnt = 0;
            simulation_curve_trigger_delay_time = 0;
            simulation_curve_release_delay_time = 0;
            key_trigger_cnt = 0;
            waveIndex = 0;
            simulation_curve_timeout = 6;
            DAC_SetChannel1Data(DAC_Align_12b_R, sineWaveTable[waveIndex]);
            HALL_LDO_EN(1);
            memset(simulation_curve_trigger_delay_time_buf, 0xff, sizeof(simulation_curve_trigger_delay_time_buf));
            memset(simulation_curve_release_delay_time_buf, 0xff, sizeof(simulation_curve_release_delay_time_buf));
            printf("Simulation Curve Mode Delay Test Start\r\n");
        } else {
            test_sta = 2;
            HALL_LDO_EN(0);
            printf("Simulation Curve Mode Delay Test Finsh\r\n");
        }
    }

    if (simulation_curve_flag) {
        if (get_tim4_dt_10us(simulation_curve_time) >= simulation_curve_timeout) {
            if (waveIndex++ >= SINE_WAVE_POINTS) {
                waveIndex = 0;
                ++simulation_curve_cnt;
            }

            if (waveIndex == 430) {
                if (simulation_curve_cnt == 2) {
                    if (multi_key_Judg_cnt == 0 && test_cnt_mode == 0) {
                        simulation_curve_flag = 0;
                        lcd_test_data_anomaly_set(2);
                    }
                }
                simulation_curve_release_flag = 1;
                simulation_curve_single_release_cnt = 0;
                multi_key_Judg_cnt = 0;
                // printf("start key lift, tim4 tick: %d\r\n", get_tim4_10us());
            }

            if (waveIndex == 967) {
                if (simulation_curve_cnt == 2) {
                    if (multi_key_Judg_cnt == 0 && test_cnt_mode == 0) {
                        simulation_curve_flag = 0;
                        lcd_test_data_anomaly_set(2);
                    }
                }
                simulation_curve_press_flag = 1;
                simulation_curve_single_trigger_cnt = 0;
                multi_key_Judg_cnt = 0;
                // printf("start key press, tim4 tick: %d\r\n", get_tim4_10us());
            }
            DAC_SetChannel1Data(DAC_Align_12b_R, sineWaveTable[waveIndex]);
            simulation_curve_time = get_tim4_10us();
            if (multi_key_Judg_cnt >= 2 && test_cnt_mode == 0) {
                simulation_curve_flag = 0;
                printf("The keyboard has a double touch issue, sending %d packets in a single press or release event.\r\n", multi_key_Judg_cnt);
                multi_key_Judg_cnt = 0;
                printf("simulation_curve_cnt == %d\r\n", simulation_curve_cnt);
                if (simulation_curve_cnt == 0) 
                    simulation_curve_cnt = 1;

                if (simulation_curve_cnt == 1) {
                    test_cnt = simulation_curve_cnt;
                } else {
                    test_cnt = simulation_curve_cnt - 1;
                }
            }
        }
    }

    if (simulation_curve_cnt >= test_cnt) {
        DAC_SetChannel1Data(DAC_Align_12b_R, 0);
        HALL_LDO_EN(0);
        simulation_curve_cnt = 0;
        simulation_curve_flag = 0;
        if (test_cnt_mode) {
            if (key_trigger_cnt > ((test_cnt << 1) + 1)) {
                printf("key trigger cnt: %d\r\n", key_trigger_cnt);
                simulation_curve_data_invalid = 1;
            } else {
                printf("key trigger cnt: %d\r\n", key_trigger_cnt);
            }
        } else {
            lcd_test_cnt_set(test_cnt);
        }

        for (uint8_t i = 0; i < test_cnt; i++) {
            printf("simulation_curve_trigger_delay_time_buf[%d]: %d\r\n", i, simulation_curve_trigger_delay_time_buf[i]);
        }
        for (uint8_t i = 0; i < test_cnt; i++) {
            printf("simulation_curve_release_delay_time_buf[%d]: %d\r\n", i, simulation_curve_release_delay_time_buf[i]);
        }
        for (uint8_t i = 0; i < test_cnt; i++) {
            simulation_curve_trigger_delay_time += simulation_curve_trigger_delay_time_buf[i];
            simulation_curve_release_delay_time += simulation_curve_release_delay_time_buf[i];
            if (simulation_curve_trigger_delay_time_buf[i] == 0xffff || simulation_curve_trigger_delay_time_buf[i] > 30000) {
                simulation_curve_data_invalid = 2;
                break;
            }
            if (simulation_curve_release_delay_time_buf[i] == 0xffff || simulation_curve_release_delay_time_buf[i] > 30000) {
                simulation_curve_data_invalid = 2;
                break;
            }
        }
        simulation_curve_trigger_delay_time = simulation_curve_trigger_delay_time / test_cnt;
        simulation_curve_release_delay_time = simulation_curve_release_delay_time / test_cnt;
        if (simulation_curve_data_invalid) {
            simulation_curve_trigger_delay_time = 0;
            simulation_curve_release_delay_time = 0;
            lcd_test_data_anomaly_set(simulation_curve_data_invalid);
        } else {
            lcd_press_delay_time_set(simulation_curve_trigger_delay_time);
            lcd_lift_delay_time_set(simulation_curve_release_delay_time);
            printf("trigger delay time: %d\r\n", simulation_curve_trigger_delay_time);
            printf("release delay time: %d\r\n", simulation_curve_release_delay_time);
        }
        if (test_cnt_mode == 0) {
            test_cnt = 100;
        } else if (test_cnt_mode == 1) {
            test_cnt = 1;
        } else if (test_cnt_mode == 2) {
            test_cnt = 10;
        } else if (test_cnt_mode == 3) {
            test_cnt = 100;
        }
    }

}

static void dac_triangle_wave_generate(void)
{
    static uint16_t min = WAVE_TRIGGER_DAC_VALUE;
    static uint16_t max = WAVE_RELEASE_DAC_VALUE;
    static uint16_t val = WAVE_TRIGGER_DAC_VALUE;
    static uint16_t step = 1;
    static uint8_t rising = 1;  // 方向标志：1=上升，0=下降
    if (triangle_wave_flag == 1) {
        test_sta = 1;
        DAC_SetChannel1Data(DAC_Align_12b_R, min);
        HALL_LDO_EN(1);
        rising = 1;
        val = WAVE_TRIGGER_DAC_VALUE;
        multi_key_Judg_cnt = 0;
        key_trigger_cnt = 0;
        triangle_wave_cnt = 0;
        triangle_wave_flag = 2;
        triangle_wave_time = get_tim4_10us();
        triangle_data_invalid = 0;
        triangle_single_trigger_cnt = 0;
        triangle_single_release_cnt = 0;
        triangle_trigger_delay_time = 0;
        triangle_release_delay_time = 0;
        memset(triangle_trigger_delay_time_buf, 0xff, sizeof(triangle_trigger_delay_time_buf));
        memset(triangle_release_delay_time_buf, 0xff, sizeof(triangle_release_delay_time_buf));
        printf("Triangle Wave Mode Delay Test Start\r\n");
    } else if ((triangle_wave_flag == 2) && (get_tim4_dt_10us(triangle_wave_time) >= 2000)) {
        triangle_wave_flag = 3;
        triangle_wave_time = get_tim4_10us();
    } else if (triangle_wave_flag == 3) {
        if (get_tim4_dt_10us(triangle_wave_time) >= 2) {
            if (val == min) {
                if (triangle_wave_cnt == 2) {
                    if (multi_key_Judg_cnt == 0 && test_cnt_mode == 0) {
                        triangle_wave_flag = 0;
                        lcd_test_data_anomaly_set(2);
                    }
                }
                triangle_wave_release_flag = 1;
                triangle_single_release_cnt = 0;
                multi_key_Judg_cnt = 0;
            }
            if (rising) {
                val += step;
                if (val >= max) {
                    rising = 0;
                }
            } else {
                val -= step;
                if (val <= min) {
                    rising = 1;
                    triangle_wave_flag = 3;
                    DAC_SetChannel1Data(DAC_Align_12b_R, val);
                    ++triangle_wave_cnt;
                    triangle_single_trigger_cnt = 0;
                    triangle_single_release_cnt = 0;
                }
            }
            if (val == max) {
                if (triangle_wave_cnt == 2) {
                    if (multi_key_Judg_cnt == 0 && test_cnt_mode == 0) {
                        triangle_wave_flag = 0;
                        lcd_test_data_anomaly_set(2);
                    }
                }
                triangle_wave_press_flag = 1;
                triangle_single_trigger_cnt = 0;
                multi_key_Judg_cnt = 0;
            }
            if (triangle_wave_flag == 3) {
                DAC_SetChannel1Data(DAC_Align_12b_R, val);
                HALL_LDO_EN(1);
                triangle_wave_time = get_tim4_10us();
            }
        }
        if (multi_key_Judg_cnt >= 2 && test_cnt_mode == 0) {
            test_sta = 2;
            triangle_wave_flag = 0;
            printf("The keyboard has a double touch issue, sending %d packets in a single press or release event.\r\n", multi_key_Judg_cnt);
            multi_key_Judg_cnt = 0;
            printf("triangle_wave_cnt == %d\r\n", triangle_wave_cnt);
            if (triangle_wave_cnt == 0) 
                triangle_wave_cnt = 1;

            if (triangle_wave_cnt == 1) {
                test_cnt = triangle_wave_cnt;
            } else {
                test_cnt = triangle_wave_cnt - 1;
            }
        }
    }

    if (triangle_wave_cnt >= test_cnt) {
        test_sta = 2;
        triangle_wave_cnt = 0;
        triangle_wave_flag = 0;
        DAC_SetChannel1Data(DAC_Align_12b_R, 0);
        HALL_LDO_EN(0);
        if (test_cnt_mode) {
            if (key_trigger_cnt > ((test_cnt << 1) + 1)) {
                printf("key trigger cnt: %d\r\n", key_trigger_cnt);
                triangle_data_invalid = 1;
            } else {
                printf("key trigger cnt: %d\r\n", key_trigger_cnt);
            }
        } else {
            lcd_test_cnt_set(test_cnt);
        }

        for (uint8_t i = 0; i < test_cnt; i++) {
            printf("triangle_trigger_delay_time_buf[%d]: %d\r\n", i, triangle_trigger_delay_time_buf[i]);
        }
        for (uint8_t i = 0; i < test_cnt; i++) {
            printf("triangle_release_delay_time_buf[%d]: %d\r\n", i, triangle_release_delay_time_buf[i]);
        }
        for (uint8_t i = 0; i < test_cnt; i++) {
            triangle_trigger_delay_time += triangle_trigger_delay_time_buf[i];
            triangle_release_delay_time += triangle_release_delay_time_buf[i];
            if (triangle_trigger_delay_time_buf[i] == 0xffff || triangle_trigger_delay_time_buf[i] > 30000) {
                triangle_data_invalid = 2;
                break;
            }
            if (triangle_release_delay_time_buf[i] == 0xffff || triangle_release_delay_time_buf[i] > 30000) {
                triangle_data_invalid = 2;
                break;
            }
        }
        triangle_trigger_delay_time = triangle_trigger_delay_time / test_cnt;
        triangle_release_delay_time = triangle_release_delay_time / test_cnt;
        if (triangle_data_invalid) {
            triangle_trigger_delay_time = 0;
            triangle_release_delay_time = 0;
            lcd_test_data_anomaly_set(triangle_data_invalid);
        } else {
            lcd_press_delay_time_set(triangle_trigger_delay_time);
            lcd_lift_delay_time_set(triangle_release_delay_time);
            printf("trigger delay time: %d\r\n", triangle_trigger_delay_time);
            printf("release delay time: %d\r\n", triangle_release_delay_time);
        }
        printf("Triangle Wave Mode Delay Test Finish\r\n");
        if (test_cnt_mode == 0) {
            test_cnt = 100;
        } else if (test_cnt_mode == 1) {
            test_cnt = 1;
        } else if (test_cnt_mode == 2) {
            test_cnt = 10;
        } else if (test_cnt_mode == 3) {
            test_cnt = 100;
        }
    }
}

static void dac_square_wave_wave_generate(void)
{
    static uint8_t trig_start_flag_temp = 0;
    if (square_wave_flag != trig_start_flag_temp) {
        trig_start_flag_temp = square_wave_flag;
        if (square_wave_flag) {
            DAC_SetChannel1Data(DAC_Align_12b_R, WAVE_TRIGGER_DAC_VALUE);
            HALL_LDO_EN(1);
            multi_key_Judg_cnt = 0;
            square_trigger_sta = 0;
            square_wave_cnt = 0;
            square_wave_time = get_sys_time_ms();
            square_data_invalid = 0;
            test_sta = 1;
            square_trigger_delay_time = 0;
            square_release_delay_time = 0;
            key_trigger_cnt = 0;
            memset(square_trigger_delay_time_buf, 0xff, sizeof(square_trigger_delay_time_buf));
            memset(square_release_delay_time_buf, 0xff, sizeof(square_release_delay_time_buf));
            printf("Square Wave Mode Delay Test Start\r\n");
        } else {
            test_sta = 2;
            HALL_LDO_EN(0);
            printf("Square Wave Mode Delay Test Finsh\r\n");
        }
    }
    if (square_wave_flag) {
        if (multi_key_Judg_cnt >= 2 && test_cnt_mode == 0) {
            printf("The keyboard has a double touch issue, sending %d packets in a single press or release event.\r\n", multi_key_Judg_cnt);
            multi_key_Judg_cnt = 0;
            test_cnt = square_wave_cnt - 1;
            lcd_test_cnt_set(test_cnt);
        }
        if (!square_trigger_sta && get_sys_dt_ms(square_wave_time) >= 30) {
            ++square_wave_cnt;
            if (square_wave_cnt > test_cnt) {
                DAC_SetChannel1Data(DAC_Align_12b_R, 0);
                HALL_LDO_EN(0);
                square_wave_cnt = 0;
                square_wave_flag = 0;
                if (test_cnt_mode) {
                    if (key_trigger_cnt > ((test_cnt << 1) + 1)) {
                        printf("key trigger cnt: %d\r\n", key_trigger_cnt);
                        square_data_invalid = 1;
                    } else {
                        printf("key trigger cnt: %d\r\n", key_trigger_cnt);
                    }
                } else {
                    lcd_test_cnt_set(test_cnt);
                }
                for (uint8_t i = 0; i < test_cnt; i++) {
                    printf("square_trigger_delay_time_buf[%d]: %d\r\n", i, square_trigger_delay_time_buf[i]);
                }
                for (uint8_t i = 0; i < test_cnt; i++) {
                    printf("square_release_delay_time_buf[%d]: %d\r\n", i, square_release_delay_time_buf[i]);
                }
                for (uint8_t i = 0; i < test_cnt; i++) {
                    square_trigger_delay_time += square_trigger_delay_time_buf[i];
                    square_release_delay_time += square_release_delay_time_buf[i];
                    if (square_trigger_delay_time_buf[i] == 0xffff || square_trigger_delay_time_buf[i] > 30000) {
                        square_data_invalid = 2;
                        break;
                    }
                    if (square_release_delay_time_buf[i] == 0xffff || square_release_delay_time_buf[i] > 30000) {
                        square_data_invalid = 2;
                        break;
                    }
                }
                square_trigger_delay_time = square_trigger_delay_time / test_cnt;
                square_release_delay_time = square_release_delay_time / test_cnt;
                if (square_data_invalid) {
                    square_trigger_delay_time = 0;
                    square_release_delay_time = 0;
                    lcd_test_data_anomaly_set(square_data_invalid);
                } else {
                    lcd_press_delay_time_set(square_trigger_delay_time);
                    lcd_lift_delay_time_set(square_release_delay_time);
                    printf("trigger delay time: %d\r\n", square_trigger_delay_time);
                    printf("release delay time: %d\r\n", square_release_delay_time);
                }
                if (test_cnt_mode == 0) {
                    test_cnt = 100;
                } else if (test_cnt_mode == 1) {
                    test_cnt = 1;
                } else if (test_cnt_mode == 2) {
                    test_cnt = 10;
                } else if (test_cnt_mode == 3) {
                    test_cnt = 100;
                }
            }
            if (square_wave_flag) {
                if (square_wave_cnt == 2) {
                    if (multi_key_Judg_cnt == 0 && test_cnt_mode == 0) {
                        square_wave_flag = 0;
                        lcd_test_data_anomaly_set(2);
                    }
                }
                DAC_SetChannel1Data(DAC_Align_12b_R, WAVE_RELEASE_DAC_VALUE);
                HALL_LDO_EN(1);
                square_single_release_flag = 1;
                square_single_release_cnt = 0;
                square_trigger_sta = 1;
                square_wave_time = get_sys_time_ms();
                multi_key_Judg_cnt = 0;
            }
        }
        if (square_trigger_sta && get_sys_dt_ms(square_wave_time) >= 30) {
            if (square_wave_cnt == 2) {
                if (multi_key_Judg_cnt == 0 && test_cnt_mode == 0) {
                    square_wave_flag = 0;
                    lcd_test_data_anomaly_set(2);
                }
            }
            if (square_wave_flag) {
                DAC_SetChannel1Data(DAC_Align_12b_R, WAVE_TRIGGER_DAC_VALUE);
                HALL_LDO_EN(1);
                square_single_press_flag = 1;
                square_single_trigger_cnt = 0;
                square_trigger_sta = 0;
                square_wave_time = get_sys_time_ms();
                multi_key_Judg_cnt = 0;
            }
        }
    }
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main( void )
{
    /* Initialize system configuration */
    SystemCoreClockUpdate( );
    Delay_Init( );
    USART_Printf_Init( 115200 );
    
    printf( "SystemClk:%d\r\n", SystemCoreClock );
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf( "Delay Tester\r\n" );

    /* Initialize TIM2, Time base: 1ms  */
    TIM2_Init((1000 - 1), ((SystemCoreClock / 1000 / 1000) - 1));

    /* Initialize TIM4, Time base: 10us */
    TIM4_Init((10 - 1), ((SystemCoreClock / 1000 / 1000) - 1));

    /* Initialize TIM3, Time base: 10us */
    TIM3_Init( 9, (SystemCoreClock / 1000 / 1000) - 1 );

    Dac_Init();	

    swkey_init();

    lcd_init();

    /* Initialize USBHS host */
    /* Note: Only CH32V305/CH32V307 support USB high-speed port. */
#if DEF_USBHS_PORT_EN
    printf( "USBHS Host Init\r\n" );
    USBHS_RCC_Init( );
    USBHS_Host_Init( ENABLE );
    memset( &RootHubDev[ DEF_USBHS_PORT_INDEX ].bStatus, 0, sizeof( ROOT_HUB_DEVICE ) );
    memset( &HostCtl[ DEF_USBHS_PORT_INDEX * DEF_ONE_USB_SUP_DEV_TOTAL ].InterfaceNum, 0, DEF_ONE_USB_SUP_DEV_TOTAL * sizeof( HOST_CTL ) );
#endif

    /* Initialize USBFS host */
#if DEF_USBFS_PORT_EN
    printf( "USBFS Host Init\r\n" );
    USBFS_RCC_Init( );
    USBFS_Host_Init( ENABLE );
    memset( &RootHubDev[ DEF_USBFS_PORT_INDEX ].bStatus, 0, sizeof( ROOT_HUB_DEVICE ) );
    memset( &HostCtl[ DEF_USBFS_PORT_INDEX * DEF_ONE_USB_SUP_DEV_TOTAL ].InterfaceNum, 0, DEF_ONE_USB_SUP_DEV_TOTAL * sizeof( HOST_CTL ) );
#endif
    
    while( 1 )
    {
        static uint8_t sta;
        if (sta != dev_sta) {
            sta = dev_sta;
            if (dev_sta) {
                lcd_device_state_set(dev_sta);
                lcd_product_name_set(product_name);
            } else {
                lcd_device_state_set(dev_sta);
                test_sta = 0;
            }
        }

        static uint8_t mode;
        if (mode != test_mode) {
            mode = test_mode;
            lcd_test_mode_set(test_mode);
        }

        static uint8_t test_sta_temp;
        if (test_sta_temp != test_sta) {
            test_sta_temp = test_sta;
            lcd_test_state_set(test_sta);
        }

        static uint32_t press_delay_time;
        if (press_delay != press_delay_time) {
            press_delay_time = press_delay;
            lcd_press_delay_time_set(press_delay);
        }

        static uint32_t lift_delay_time;
        if (lift_delay != lift_delay_time) {
            lift_delay_time = lift_delay;
            lcd_lift_delay_time_set(lift_delay);
        }

        swkey_loop();

        USBH_MainDeal( );

        if (test_mode == 0) {
            dac_simulation_curve_generate();
        } else if (test_mode == 1) {
            dac_square_wave_wave_generate();
        } else if (test_mode == 2) {
            dac_triangle_wave_generate();
        }

    }
}



