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

#ifndef __KEY_H
#define __KEY_H

#include "ch32v30x_conf.h"

typedef struct {
    GPIO_TypeDef   *GPIOx;
    uint16_t        Pin;
    BitAction       Bit;
} _Hw_Cs_Gpio_t;

// 时间阈值定义 (单位: ms)
#define SHORT_PRESS_MAX  400   // 短按最大持续时间
#define LONG_PRESS_MIN   1000  // 长按最小持续时间
#define DOUBLE_CLICK_MAX 300   // 双击最大间隔
#define DEBOUNCE_TIME    20    // 按键防抖时间
#define HOLD_REPEAT_TIME 100   // 长按保持重复时间

#define SW_KEY_NUM       4     // 按键数量

#define LDO_EN_PORT       GPIOA
#define LDO_EN_PIN        (GPIO_Pin_0)

#define HALL_LDO_EN(en)  GPIO_WriteBit(LDO_EN_PORT, LDO_EN_PIN, en ? Bit_SET:Bit_RESET);

// 按键事件类型
typedef enum {
    BUTTON_NONE,
    BUTTON_SHORT_PRESS,
    BUTTON_LONG_PRESS_START,
    BUTTON_LONG_PRESS_END,
    BUTTON_DOUBLE_CLICK
} ButtonEvent;

// 按键状态机
typedef enum {
    BUTTON_STATE_IDLE,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_RELEASED,
    BUTTON_STATE_WAIT_DOUBLE,
    BUTTON_STATE_LONG_PRESS
} ButtonState;

// 按键配置结构
typedef struct {
    ButtonState state;
    uint32_t pressStartTime;
    uint32_t releaseTime;
    uint32_t lastHoldTime;
    uint8_t clickCount;
    ButtonEvent lastEvent;
} ButtonConfig;

typedef void (*func)(void);

void swkey_init(void);
void swkey_loop(void);

#endif