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

uint8_t  product_name[20];
uint8_t  dev_sta = 0;
uint32_t press_delay = 9999;
uint32_t lift_delay  = 9999;
uint8_t  test_mode = 0;
uint8_t  test_sta = 0;
uint16_t key_trigger_cnt = 0;
uint16_t key_release_cnt = 0;

uint8_t  square_wave_flag = 0;
uint16_t square_wave_cnt = 0;
uint32_t square_wave_time = 0;
uint8_t  square_single_press_flag = 0;
uint8_t  square_single_release_flag = 0;
uint32_t square_single_trigger_cnt  = 0;
uint32_t square_single_release_cnt  = 0;
uint16_t square_trigger_delay_time_buf[DELAY_TEST_CNT] = {0};
uint16_t square_release_delay_time_buf[DELAY_TEST_CNT] = {0};
uint32_t square_trigger_delay_time = 0;
uint32_t square_release_delay_time = 0;
uint8_t  square_data_invalid = 0;
uint8_t  square_trigger_sta = 0;
uint32_t square_test_cnt = DELAY_TEST_CNT;

uint8_t  triangle_wave_flag = 0;
uint32_t triangle_wave_cnt = 0;
uint32_t triangle_wave_time = 0;
uint8_t  triangle_wave_press_flag = 0;
uint8_t  triangle_wave_release_flag = 0;
uint32_t triangle_single_trigger_cnt = 0;
uint32_t triangle_single_release_cnt = 0;
uint16_t triangle_trigger_delay_time_buf[DELAY_TEST_CNT] = {0};
uint16_t triangle_release_delay_time_buf[DELAY_TEST_CNT] = {0};
uint32_t triangle_trigger_delay_time = 0;
uint32_t triangle_release_delay_time = 0;
uint8_t  triangle_data_invalid = 0;

uint8_t  simulation_curve_flag = 0;
uint32_t simulation_curve_cnt = 0;
uint32_t simulation_curve_time = 0;
uint8_t  simulation_curve_press_flag = 0;
uint8_t  simulation_curve_release_flag = 0;
uint32_t simulation_curve_single_trigger_cnt = 0;
uint32_t simulation_curve_single_release_cnt = 0;
uint16_t simulation_curve_trigger_delay_time_buf[DELAY_TEST_CNT] = {0};
uint16_t simulation_curve_release_delay_time_buf[DELAY_TEST_CNT] = {0};
uint32_t simulation_curve_trigger_delay_time = 0;
uint32_t simulation_curve_release_delay_time = 0;
uint8_t  simulation_curve_data_invalid = 0;
uint32_t simulation_curve_timeout = 0;
uint32_t simulation_curve_test_cnt = DELAY_TEST_CNT;

#if 1
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

    2000, 2010, 2020, 2030, 2040, 2050, 2060, 2070, 2080, 2090,
    2100, 2110, 2120, 2130, 2140, 2150, 2160, 2170, 2180, 2190,
    2200, 2210, 2220, 2230, 2240, 2250, 2260, 2270, 2280, 2290,
    2300, 2310, 2320, 2330, 2340, 2350, 2360, 2370, 2380, 2390,
    2400, 2410, 2420, 2430, 2440, 2450, 2460, 2470, 2480, 2490,
    2500, 2510, 2520, 2530, 2540, 2550, 2560, 2570, 2580, 2590,
    2600, 2610, 2620, 2630, 2640, 2650, 2660, 2670, 2680, 2690,
    2700, 2710, 2720, 2730, 2740, 2750, 2760, 2770, 2780, 2790,
    2800, 2810, 2820, 2830, 2840, 2850, 2860, 2870, 2880, 2890,
    2900, 2910, 2920, 2930, 2940, 2950, 2960, 2970, 2980, 2990,
    3000, 3010, 3020, 3030, 3040, 3050, 3060, 3070, 3080, 3090,
    3100, 3110, 3120, 3130, 3140, 3150, 3160, 3170, 3180, 3190,
    3200, 3210, 3220, 3230, 3240, 3250, 3260, 3270, 3280, 3290,
    3300, 3310, 3320, 3330, 3340, 3350, 3360, 3370, 3380, 3390,
    3400, 3410, 3420, 3430, 3440, 3450, 3460, 3470, 3480, 3490,

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

    3470, 3440, 3410, 3380, 3350, 3320, 3290, 3260, 3230, 3200,
    3170, 3140, 3110, 3080, 3050, 3020, 2990, 2960, 2930, 2900,
    2870, 2840, 2810, 2780, 2750, 2720, 2690, 2660, 2630, 2600,
    2570, 2540, 2510, 2480, 2450, 2420, 2390, 2360, 2330, 2300,
    2270, 2240, 2210, 2180, 2150, 2120, 2090, 2060, 2030, 2000,
};
#endif

#if 0
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

    2004, 2008, 2013, 2018, 2024, 2030, 2037, 2044, 2052, 2060, 
    2071, 2082, 2093, 2104, 2115, 2126, 2137, 2148, 2159, 2170, 
    2181, 2192, 2203, 2214, 2225, 2236, 2247, 2258, 2269, 2280, 
    2291, 2302, 2313, 2324, 2335, 2346, 2357, 2368, 2379, 2395,
    2411, 2422, 2433, 2444, 2455, 2466, 2477, 2488, 2499, 2510, 
    2521, 2532, 2543, 2554, 2565, 2576, 2587, 2598, 2609, 2620, 
    2631, 2642, 2653, 2664, 2675, 2686, 2697, 2708, 2719, 2730, 
    2741, 2752, 2763, 2774, 2785, 2796, 2807, 2818, 2829, 2840, 
    2851, 2862, 2873, 2884, 2895, 2906, 2917, 2928, 2939, 2950, 
    2961, 2972, 2983, 2994, 3005, 3016, 3027, 3038, 3049, 3060, 
    3071, 3082, 3093, 3104, 3115, 3126, 3137, 3148, 3159, 3170, 
    3181, 3192, 3203, 3214, 3225, 3236, 3247, 3258, 3269, 3280, 
    3291, 3302, 3313, 3324, 3335, 3346, 3357, 3368, 3379, 3390, 
    3401, 3412, 3423, 3434, 3445, 3456, 3467, 3478, 3489, 3500,

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
    3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500, 3500,

    3470, 3440, 3410, 3380, 3350, 3320, 3290, 3260, 3230, 3200,
    3160, 3120, 3080, 3040, 3000, 2960, 2920, 2880, 2840, 2800, 
    2760, 2720, 2680, 2640, 2600, 2560, 2520, 2480, 2440, 2400, 
    2360, 2320, 2280, 2240, 2200, 2160, 2120, 2080, 2040, 2000,
};
#endif

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
            simulation_curve_cnt = 0;
            simulation_curve_time = get_tim4_10us();
            simulation_curve_data_invalid = 0;
            test_sta = 1;
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
                simulation_curve_single_trigger_cnt = 0;
                simulation_curve_single_release_cnt = 0;
            }
            if (waveIndex == 401) {
                simulation_curve_release_flag = 1;
            }

            if (waveIndex == 950) {
                simulation_curve_press_flag = 1;
            }
            DAC_SetChannel1Data(DAC_Align_12b_R, sineWaveTable[waveIndex]);
            simulation_curve_time = get_tim4_10us();
        }
    }

    if (simulation_curve_cnt > DELAY_TEST_CNT) {
        DAC_SetChannel1Data(DAC_Align_12b_R, 0);
        HALL_LDO_EN(0);
        simulation_curve_cnt = 0;
        simulation_curve_flag = 0;
        if (key_trigger_cnt > ((DELAY_TEST_CNT << 1) + 20)) {
            printf("key trigger cnt: %d\r\n", key_trigger_cnt);
            simulation_curve_data_invalid = 1;
        } else if (key_trigger_cnt > (DELAY_TEST_CNT << 1)) {
            printf("key trigger cnt: %d\r\n", key_trigger_cnt);
            simulation_curve_test_cnt = (key_trigger_cnt >> 1);
        }

        for (uint8_t i = 0; i < simulation_curve_test_cnt; i++) {
            printf("simulation_curve_trigger_delay_time_buf[%d]: %d\r\n", i, simulation_curve_trigger_delay_time_buf[i]);
        }
        for (uint8_t i = 0; i < simulation_curve_test_cnt; i++) {
            printf("simulation_curve_release_delay_time_buf[%d]: %d\r\n", i, simulation_curve_release_delay_time_buf[i]);
        }
        for (uint8_t i = 0; i < simulation_curve_test_cnt; i++) {
            simulation_curve_trigger_delay_time += simulation_curve_trigger_delay_time_buf[i];
            simulation_curve_release_delay_time += simulation_curve_release_delay_time_buf[i];
            if (simulation_curve_trigger_delay_time_buf[i] == 0xffff) {
                simulation_curve_data_invalid = 2;
                break;
            }
            if (simulation_curve_release_delay_time_buf[i] == 0xffff) {
                simulation_curve_data_invalid = 2;
                break;
            }
        }
        simulation_curve_trigger_delay_time = simulation_curve_trigger_delay_time / simulation_curve_test_cnt;
        simulation_curve_release_delay_time = simulation_curve_release_delay_time / simulation_curve_test_cnt;
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
        key_trigger_cnt = 0;
        triangle_wave_flag = 2;
        triangle_wave_time = get_tim4_10us();
        triangle_data_invalid = 0;
        triangle_single_trigger_cnt = 0;
        triangle_single_release_cnt = 0;
        triangle_trigger_delay_time = 0;
        triangle_release_delay_time = 0;
        memset(triangle_trigger_delay_time_buf, 0xff, sizeof(triangle_trigger_delay_time_buf));
        memset(triangle_trigger_delay_time_buf, 0xff, sizeof(triangle_release_delay_time_buf));
        printf("Triangle Wave Mode Delay Test Start\r\n");
    } else if ((triangle_wave_flag == 2) && (get_tim4_dt_10us(triangle_wave_time) >= 2000)) {
        triangle_wave_flag = 3;
        triangle_wave_time = get_tim4_10us();
    } else if (triangle_wave_flag == 3) {
        if (get_tim4_dt_10us(triangle_wave_time) >= 2) {
            if (val == min) {
                triangle_wave_release_flag = 1;
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
                triangle_wave_press_flag = 1;
            }
            if (triangle_wave_flag == 3) {
                DAC_SetChannel1Data(DAC_Align_12b_R, val);
                HALL_LDO_EN(1);
                triangle_wave_time = get_tim4_10us();
            }
        }
    }

    if (triangle_wave_cnt >= DELAY_TEST_CNT) {
        test_sta = 2;
        triangle_wave_cnt = 0;
        triangle_wave_flag = 0;
        DAC_SetChannel1Data(DAC_Align_12b_R, 0);
        HALL_LDO_EN(0);
        if (key_trigger_cnt > (DELAY_TEST_CNT << 1)) {
            printf("key trigger cnt: %d\r\n", key_trigger_cnt);
            triangle_data_invalid = 1;
        }
        for (uint8_t i = 0; i < DELAY_TEST_CNT; i++) {
            printf("triangle_trigger_delay_time_buf[%d]: %d\r\n", i, triangle_trigger_delay_time_buf[i]);
        }
        for (uint8_t i = 0; i < DELAY_TEST_CNT; i++) {
            printf("triangle_release_delay_time_buf[%d]: %d\r\n", i, triangle_release_delay_time_buf[i]);
        }
        for (uint8_t i = 0; i < DELAY_TEST_CNT; i++) {
            triangle_trigger_delay_time += triangle_trigger_delay_time_buf[i];
            triangle_release_delay_time += triangle_release_delay_time_buf[i];
            if (triangle_trigger_delay_time_buf[i] == 0xffff) {
                triangle_data_invalid = 2;
                break;
            }
            if (triangle_release_delay_time_buf[i] == 0xffff) {
                triangle_data_invalid = 2;
                break;
            }
        }
        triangle_trigger_delay_time = triangle_trigger_delay_time / DELAY_TEST_CNT;
        triangle_release_delay_time = triangle_release_delay_time / DELAY_TEST_CNT;
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
    }
}

static void dac_square_wave_wave_generate(void)
{
    static uint8_t trig_start_flag_temp = 0;
    if (square_wave_flag != trig_start_flag_temp) {
        trig_start_flag_temp = square_wave_flag;
        if (square_wave_flag) {
            square_trigger_sta = 1;
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
        if (!square_trigger_sta && get_sys_dt_ms(square_wave_time) >= 30) {
            DAC_SetChannel1Data(DAC_Align_12b_R, WAVE_RELEASE_DAC_VALUE);
            HALL_LDO_EN(1);
            square_single_release_flag = 1;
            square_single_release_cnt = 0;
            square_trigger_sta = 1;
            square_wave_time = get_sys_time_ms();
        }
        if (square_trigger_sta && get_sys_dt_ms(square_wave_time) >= 30) {
            ++square_wave_cnt;
            if (square_wave_cnt > DELAY_TEST_CNT) {
                DAC_SetChannel1Data(DAC_Align_12b_R, 0);
                HALL_LDO_EN(0);
                square_wave_cnt = 0;
                square_wave_flag = 0;
                if (key_trigger_cnt > ((DELAY_TEST_CNT << 1) + 20)) {
                    printf("key trigger cnt: %d\r\n", key_trigger_cnt);
                    square_data_invalid = 1;
                } else if (key_trigger_cnt > (DELAY_TEST_CNT << 1)) {
                    printf("key trigger cnt: %d\r\n", key_trigger_cnt);
                    square_test_cnt = (key_trigger_cnt >> 1);
                }
                for (uint8_t i = 0; i < square_test_cnt; i++) {
                    printf("square_trigger_delay_time_buf[%d]: %d\r\n", i, square_trigger_delay_time_buf[i]);
                }
                for (uint8_t i = 0; i < square_test_cnt; i++) {
                    printf("square_release_delay_time_buf[%d]: %d\r\n", i, square_release_delay_time_buf[i]);
                }
                for (uint8_t i = 0; i < square_test_cnt; i++) {
                    square_trigger_delay_time += square_trigger_delay_time_buf[i];
                    square_release_delay_time += square_release_delay_time_buf[i];
                    if (square_trigger_delay_time_buf[i] == 0xffff) {
                        square_data_invalid = 2;
                        break;
                    }
                    if (square_release_delay_time_buf[i] == 0xffff) {
                        square_data_invalid = 2;
                        break;
                    }
                }
                square_trigger_delay_time = square_trigger_delay_time / square_test_cnt;
                square_release_delay_time = square_release_delay_time / square_test_cnt;
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
            }
            if (square_wave_flag) {
                DAC_SetChannel1Data(DAC_Align_12b_R, WAVE_TRIGGER_DAC_VALUE);
                HALL_LDO_EN(1);
                square_single_press_flag = 1;
                square_single_trigger_cnt = 0;
                square_trigger_sta = 0;
                square_wave_time = get_sys_time_ms();
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

        if (test_mode) {
            dac_simulation_curve_generate();
        } else {
            dac_square_wave_wave_generate();
        }

    }
}



