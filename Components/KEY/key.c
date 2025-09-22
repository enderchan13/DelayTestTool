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

#include "key.h"
#include "tim.h"
#include "lcd.h"

const _Hw_Cs_Gpio_t hw_sw_cfg_tbl[SW_KEY_NUM] = {
    {GPIOC, GPIO_Pin_1, Bit_RESET},
    {GPIOC, GPIO_Pin_2, Bit_RESET},
    {GPIOC, GPIO_Pin_8, Bit_RESET},
    {GPIOC, GPIO_Pin_9, Bit_RESET},
};

// 按键配置数组
static ButtonConfig buttons[SW_KEY_NUM] = {
    { BUTTON_STATE_IDLE, 0, 0, 0, 0, BUTTON_NONE },
    { BUTTON_STATE_IDLE, 0, 0, 0, 0, BUTTON_NONE },
    { BUTTON_STATE_IDLE, 0, 0, 0, 0, BUTTON_NONE },
    { BUTTON_STATE_IDLE, 0, 0, 0, 0, BUTTON_NONE }
};

extern uint8_t  square_wave_flag;
extern uint8_t  triangle_wave_flag;
extern uint8_t  simulation_curve_flag;
extern uint8_t  test_mode;
extern uint8_t  test_sta;
extern uint8_t  test_cnt;
extern uint8_t  test_cnt_mode;

static void swkey1_short_press_handle(void)
{
    printf("swkey1_short_press_handle\r\n");
    if (simulation_curve_flag || square_wave_flag || triangle_wave_flag)
        return;

    if (++test_cnt_mode >= 4) {
        test_cnt_mode = 0;
    }
    if (test_cnt_mode == 0) {
        test_cnt = 100;
        lcd_test_cnt_set(0);
    } else if (test_cnt_mode == 1) {
        test_cnt = 1;
        lcd_test_cnt_set(1);
    } else if (test_cnt_mode == 2) {
        test_cnt = 10;
        lcd_test_cnt_set(10);
    } else if (test_cnt_mode == 3) {
        test_cnt = 100;
        lcd_test_cnt_set(100);
    }
    
}

static void swkey1_long_press_start_handle(void)
{
    printf("swkey1_long_press_start_handle\r\n");
}

static void swkey1_long_press_end_handle(void)
{
    printf("swkey1_long_press_end_handle\r\n");
}

static void swkey1_double_click_handle(void)
{
    printf("swkey1_double_click_handle\r\n");
}

static void swkey2_short_press_handle(void)
{
    printf("swkey2_short_press_handle\r\n");
    if (simulation_curve_flag || square_wave_flag || triangle_wave_flag)
        return;

    if (++test_mode > 2)
        test_mode = 0;
}

static void swkey2_long_press_start_handle(void)
{
    printf("swkey2_long_press_start_handle\r\n");
}

static void swkey2_long_press_end_handle(void)
{
    printf("swkey2_long_press_end_handle\r\n");
}

static void swkey2_double_click_handle(void)
{
    printf("swkey2_double_click_handle\r\n");
}

static void swkey3_short_press_handle(void)
{
    printf("swkey3_short_press_handle\r\n");
    if (test_mode == 0) {
        simulation_curve_flag = 1;
    } else if (test_mode == 1) {
        square_wave_flag = 1;
    } else if (test_mode == 2) {
        triangle_wave_flag = 1;
    }
    if (test_cnt_mode == 0) {
        test_cnt = 100;
        lcd_test_cnt_set(0);
    } else if (test_cnt_mode == 1) {
        test_cnt = 1;
        lcd_test_cnt_set(1);
    } else if (test_cnt_mode == 2) {
        test_cnt = 10;
        lcd_test_cnt_set(10);
    } else if (test_cnt_mode == 3) {
        test_cnt = 100;
        lcd_test_cnt_set(100);
    }
}

static void swkey3_long_press_start_handle(void)
{
    printf("swkey3_long_press_start_handle\r\n");
}

static void swkey3_long_press_end_handle(void)
{
    printf("swkey3_long_press_end_handle\r\n");
}

static void swkey3_double_click_handle(void)
{
    printf("swkey3_double_click_handle\r\n");
}

static void swkey4_short_press_handle(void)
{
    printf("swkey4_short_press_handle\r\n");
    simulation_curve_flag = 0;
    square_wave_flag = 0;
    triangle_wave_flag = 0;
    HALL_LDO_EN(0);
    test_sta = 2;
    lcd_test_data_anomaly_set(2);
}

static void swkey4_long_press_start_handle(void)
{
    printf("swkey4_long_press_start_handle\r\n");
}

static void swkey4_long_press_end_handle(void)
{
    printf("swkey4_long_press_end_handle\r\n");
}

static void swkey4_double_click_handle(void)
{
    printf("swkey4_double_click_handle\r\n");
}

// 按键功能映射
static func buttonFunction[SW_KEY_NUM][4] = {
    // 按键处理: 短按, 长按开始, 长按结束, 双击
    { swkey1_short_press_handle, swkey1_long_press_start_handle, swkey1_long_press_end_handle, swkey1_double_click_handle },
    { swkey2_short_press_handle, swkey2_long_press_start_handle, swkey2_long_press_end_handle, swkey2_double_click_handle },
    { swkey3_short_press_handle, swkey3_long_press_start_handle, swkey3_long_press_end_handle, swkey3_double_click_handle },
    { swkey4_short_press_handle, swkey4_long_press_start_handle, swkey4_long_press_end_handle, swkey4_double_click_handle }
};

// 按键状态处理
static ButtonEvent button_event_process(uint8_t id)
{
    ButtonConfig* btn = &buttons[id];
    uint32_t now = get_sys_time_ms();
    uint8_t btnStateNow = GPIO_ReadInputDataBit(hw_sw_cfg_tbl[id].GPIOx, hw_sw_cfg_tbl[id].Pin);
    ButtonEvent event = BUTTON_NONE;

    // 按键状态机
    switch (btn->state) {
        case BUTTON_STATE_IDLE:
            if (btnStateNow == RESET) { // 按键按下
                btn->pressStartTime = now;
                btn->state = BUTTON_STATE_PRESSED;
            }
            break;

        case BUTTON_STATE_PRESSED:
            if (btnStateNow != RESET) { // 按键释放
                // 防抖处理
                if ((now - btn->pressStartTime) > DEBOUNCE_TIME) {
                    btn->releaseTime = now;
                    btn->clickCount++;
                    btn->state = BUTTON_STATE_RELEASED;
                } else {
                    btn->state = BUTTON_STATE_IDLE; // 抖动，忽略
                }
            }
            // 检查长按开始
            else if ((now - btn->pressStartTime) > LONG_PRESS_MIN) {
                btn->state = BUTTON_STATE_LONG_PRESS;
                event = BUTTON_LONG_PRESS_START;
                btn->lastHoldTime = now;
            }
            break;

        case BUTTON_STATE_RELEASED:
            // 等待稳定
            if ((now - btn->releaseTime) > DEBOUNCE_TIME) {
                if (btn->clickCount == 1) {
                    // 启动双击检测
                    btn->state = BUTTON_STATE_WAIT_DOUBLE;
                } else if (btn->clickCount == 2) {
                    btn->state = BUTTON_STATE_IDLE;
                    btn->clickCount = 0;
                    event = BUTTON_DOUBLE_CLICK;
                }
            }
            break;

        case BUTTON_STATE_WAIT_DOUBLE:
            // 检测第二次按下
            if (btnStateNow == RESET) {
                btn->pressStartTime = now;
                btn->state = BUTTON_STATE_PRESSED;
            }
            // 双击超时
            else if ((now - btn->releaseTime) > DOUBLE_CLICK_MAX) {
                btn->state = BUTTON_STATE_IDLE;
                btn->clickCount = 0;

                // 检查是否为短按
                if ((btn->releaseTime - btn->pressStartTime) < SHORT_PRESS_MAX) {
                    event = BUTTON_SHORT_PRESS;
                }
            }
            break;

        case BUTTON_STATE_LONG_PRESS:
            // 长按保持重复事件
            if ((now - btn->lastHoldTime) > HOLD_REPEAT_TIME) {
                event = BUTTON_LONG_PRESS_START; // 重复触发长按保持
                btn->lastHoldTime = now;
            }

            // 长按释放
            if (btnStateNow != RESET) { // 按键释放
                btn->state = BUTTON_STATE_IDLE;
                event = BUTTON_LONG_PRESS_END;
            }
            break;
    }

    btn->lastEvent = event;
    return event;
}

void swkey_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    /* GPIOx Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    for (uint8_t i = 0; i < SW_KEY_NUM; i++) {
        GPIO_InitStructure.GPIO_Pin = hw_sw_cfg_tbl[i].Pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(hw_sw_cfg_tbl[i].GPIOx, &GPIO_InitStructure);
    }
}

// 按键处理主函数
void swkey_loop(void)
{
    // 处理所有按键
    for (int i = 0; i < SW_KEY_NUM; i++) {
        // 处理按键事件
        ButtonEvent event = button_event_process(i);
        if (event != BUTTON_NONE) {
            // 处理不同按键事件
            switch (event) {
                case BUTTON_SHORT_PRESS:
                    buttonFunction[i][0]();
                    break;

                case BUTTON_LONG_PRESS_START:
                    buttonFunction[i][1]();
                    break;

                case BUTTON_LONG_PRESS_END:
                    buttonFunction[i][2]();
                    break;

                case BUTTON_DOUBLE_CLICK:
                    buttonFunction[i][3]();
                    break;

                default:
                    break;
            }
        }
    }
}
