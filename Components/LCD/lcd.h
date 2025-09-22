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

#ifndef __LCD_H
#define __LCD_H

#include "ch32v30x_conf.h"
#include "font/Lcd_Font.h"

#define LCD_LED_PORT      GPIOA
#define LCD_LED_PIN       (GPIO_Pin_2)

#define LCD_DC_PORT       GPIOC
#define LCD_DC_PIN        (GPIO_Pin_5)

#define LCD_RST_PORT      GPIOC
#define LCD_RST_PIN       (GPIO_Pin_4)

#define LCD_CS_PORT       GPIOB
#define LCD_CS_PIN        (GPIO_Pin_0)

#define SPI1_DMA_RX_CH   DMA1_Channel4
#define SPI1_DMA_TX_CH   DMA1_Channel5
#define SPI1_DMA_RC_FLAG DMA1_FLAG_TC4
#define SPI1_DMA_TC_FLAG DMA1_FLAG_TC5

#define LCD_LEDA_EN(en)     GPIO_WriteBit(LCD_LED_PORT, LCD_LED_PIN, en ? Bit_RESET:Bit_SET);
#define LCD_RESET_EN(en)    GPIO_WriteBit(LCD_RST_PORT, LCD_RST_PIN, en ? Bit_RESET:Bit_SET);
#define LCD_CS_EN(en)       GPIO_WriteBit(LCD_CS_PORT, LCD_CS_PIN, en ? Bit_RESET:Bit_SET);
#define LCD_RS_EN(en)       GPIO_WriteBit(LCD_DC_PORT, LCD_DC_PIN, en ? Bit_SET:Bit_RESET);

/* 屏幕驱动SOC分辨率（根据屏幕驱动SOC实际参数修改，如ST7789） */
#define SOC_WIDTH            240
#define SOC_HEIGHT           320

/* 屏幕分辨率（根据屏幕实际参数修改） */
#define LCD_WIDTH            135
#define LCD_HEIGHT           240

#if (LCD_WIDTH % 2)
#define LCD_WIDTH_START ((SOC_WIDTH - LCD_WIDTH - 1) >> 1)
#define LCD_WIDTH_END   ((SOC_WIDTH - ((SOC_WIDTH - LCD_WIDTH - 1) >> 1)) - 1 - 1)
#else
#define LCD_WIDTH_START ((SOC_WIDTH - LCD_WIDTH) >> 1)
#define LCD_WIDTH_END   ((SOC_WIDTH - ((SOC_WIDTH - LCD_WIDTH) >> 1)) - 1)
#endif

#if (LCD_HEIGHT % 2)
#define LCD_HEIGHT_START ((SOC_HEIGHT - LCD_HEIGHT - 1) >> 1)
#define LCD_HEIGHT_END   ((SOC_HEIGHT - ((SOC_HEIGHT - LCD_HEIGHT - 1) >> 1)) - 1 - 1)
#else
#define LCD_HEIGHT_START ((SOC_HEIGHT - LCD_HEIGHT) >> 1)
#define LCD_HEIGHT_END   ((SOC_HEIGHT - ((SOC_HEIGHT - LCD_HEIGHT) >> 1)) - 1)
#endif

#define RGB565(r, g, b)      (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3))

#define LCD_COLOR_RED        RGB565(255, 0, 0)
#define LCD_COLOR_GREEN      RGB565(0, 255, 0)
#define LCD_COLOR_BLUE       RGB565(0, 0, 255)
#define LCD_COLOR_YELLOW     RGB565(127, 127, 0)
#define LCD_COLOR_PURPLE     RGB565(127, 0, 127)
#define LCD_COLOR_CYAN       RGB565(0, 127, 127)
#define LCD_COLOR_ORANGE     RGB565(255, 140, 0)
#define LCD_COLOR_WHITE      RGB565(255, 255, 255)
#define LCD_COLOR_BLACK      RGB565(0, 0, 0)

typedef enum {
    FONT_12x6,
    FONT_16x8,
    FONT_24x12,
    FONT_32x16,
} FontType;

void lcd_init(void);
void lcd_press_delay_time_set(uint32_t press_delay);
void lcd_lift_delay_time_set(uint32_t lift_delay);
void lcd_product_name_set(uint8_t product_name[]);
void lcd_device_state_set(uint8_t sta);
void lcd_test_mode_set(uint8_t mode);
void lcd_test_state_set(uint8_t sta);
void lcd_test_data_anomaly_set(uint8_t sta);
void lcd_test_cnt_set(uint8_t cnt);

#endif