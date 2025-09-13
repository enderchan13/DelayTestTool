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

#include "lcd.h"
#include "lcd_image_resources.h"

const uint8_t* image_main_delay_num_addr[10] = {
    gImage_main_delay_number_0,
    gImage_main_delay_number_1,
    gImage_main_delay_number_2,
    gImage_main_delay_number_3,
    gImage_main_delay_number_4,
    gImage_main_delay_number_5,
    gImage_main_delay_number_6,
    gImage_main_delay_number_7,
    gImage_main_delay_number_8,
    gImage_main_delay_number_9,
};

const uint8_t* image_main_device_uppercase_addr[26] = {
    gImage_main_device_uppercase_A,
    gImage_main_device_uppercase_B,
    gImage_main_device_uppercase_C,
    gImage_main_device_uppercase_D,
    gImage_main_device_uppercase_E,
    gImage_main_device_uppercase_F,
    gImage_main_device_uppercase_G,
    gImage_main_device_uppercase_H,
    gImage_main_device_uppercase_I,
    gImage_main_device_uppercase_J,
    gImage_main_device_uppercase_K,
    gImage_main_device_uppercase_L,
    gImage_main_device_uppercase_M,
    gImage_main_device_uppercase_N,
    gImage_main_device_uppercase_O,
    gImage_main_device_uppercase_P,
    gImage_main_device_uppercase_Q,
    gImage_main_device_uppercase_R,
    gImage_main_device_uppercase_S,
    gImage_main_device_uppercase_T,
    gImage_main_device_uppercase_U,
    gImage_main_device_uppercase_V,
    gImage_main_device_uppercase_W,
    gImage_main_device_uppercase_X,
    gImage_main_device_uppercase_Y,
    gImage_main_device_uppercase_Z,
};

const uint8_t* image_main_device_lowercase_addr[26] = {
    gImage_main_device_lowercase_a,
    gImage_main_device_lowercase_b,
    gImage_main_device_lowercase_c,
    gImage_main_device_lowercase_d,
    gImage_main_device_lowercase_e,
    gImage_main_device_lowercase_f,
    gImage_main_device_lowercase_g,
    gImage_main_device_lowercase_h,
    gImage_main_device_lowercase_i,
    gImage_main_device_lowercase_j,
    gImage_main_device_lowercase_k,
    gImage_main_device_lowercase_l,
    gImage_main_device_lowercase_m,
    gImage_main_device_lowercase_n,
    gImage_main_device_lowercase_o,
    gImage_main_device_lowercase_p,
    gImage_main_device_lowercase_q,
    gImage_main_device_lowercase_r,
    gImage_main_device_lowercase_s,
    gImage_main_device_lowercase_t,
    gImage_main_device_lowercase_u,
    gImage_main_device_lowercase_v,
    gImage_main_device_lowercase_w,
    gImage_main_device_lowercase_x,
    gImage_main_device_lowercase_y,
    gImage_main_device_lowercase_z,
};

const uint8_t* image_main_device_num_addr[10] = {
    gImage_main_device_number_0,
    gImage_main_device_number_1,
    gImage_main_device_number_2,
    gImage_main_device_number_3,
    gImage_main_device_number_4,
    gImage_main_device_number_5,
    gImage_main_device_number_6,
    gImage_main_device_number_7,
    gImage_main_device_number_8,
    gImage_main_device_number_9,
};

uint8_t* product_name_image_addr[20];
uint32_t product_name_image_x0[20];
uint8_t* press_delay_image_addr[5];
uint8_t* lift_delay_image_addr[5];

/*********************************************************************
 * @fn      SPI1_Init
 * 
 * @brief   Initialize SPI1
 * 
 * @return  none
 */
static void SPI1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    SPI_InitTypeDef  SPI_InitStructure  = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = LCD_LED_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_LED_PORT, &GPIO_InitStructure);
    LCD_LEDA_EN(1);

    GPIO_InitStructure.GPIO_Pin   = LCD_DC_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_DC_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = LCD_RST_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_RST_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = LCD_CS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_CS_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Direction         = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial     = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);
}

/*********************************************************************
 * @fn      SPI1_DMA_Init
 * 
 * @brief   Initialize DMA for SPI1
 * 
 * @return  none
 */
static void SPI1_DMA_Init()
{
    DMA_InitTypeDef DMA_InitStructure = {0};

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(SPI1_DMA_TX_CH);

    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DATAR;
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)NULL;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize         = 0;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
    DMA_Init(SPI1_DMA_TX_CH, &DMA_InitStructure);

    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
}

/*********************************************************************
 * @fn      SPI1_Write
 * 
 * @brief   send one byte
 * 
 * @param   data -  the data to send
 * 
 * @return  none
 */
static void SPI1_Write(uint8_t data)
{
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
        ;
    SPI_I2S_SendData(SPI1, data);

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET)
        ;
}

/*********************************************************************
 * @fn      SPI1_Write_DMA
 * 
 * @brief   send a lot of data 
 * 
 * @param   data -  the data to be send 
 *          len -  the length of data
 * 
 * @return  none
 */
static void SPI1_Write_DMA(uint8_t *data, uint16_t len)
{
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET)
        ;

    DMA_Cmd(SPI1_DMA_TX_CH, DISABLE);
    DMA_ClearFlag(SPI1_DMA_TC_FLAG);
    SPI1_DMA_TX_CH->MADDR = (uint32_t)data;
    SPI1_DMA_TX_CH->CNTR  = len;

    DMA_Cmd(SPI1_DMA_TX_CH, ENABLE);
}

static void lcd_send_command(uint8_t cmd)
{
    LCD_RS_EN(0);
    LCD_CS_EN(1);
    SPI1_Write(cmd);
    LCD_CS_EN(0);
}

static void lcd_send_data(uint8_t data)
{
    LCD_RS_EN(1);
    LCD_CS_EN(1);
    SPI1_Write(data);
    LCD_CS_EN(0);
}

static void lcd_reset(void)
{
    LCD_RESET_EN(1);
    Delay_Ms(100);
    LCD_RESET_EN(0);
    Delay_Ms(100);
}

// 设置显示区域（起始/结束坐标）
static void lcd_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    lcd_send_command(0x2A);     // 列地址设置
    lcd_send_data(x0 >> 8);
    lcd_send_data(x0 & 0xFF);
    lcd_send_data(x1 >> 8);
    lcd_send_data(x1 & 0xFF);

    lcd_send_command(0x2B);     // 行地址设置
    lcd_send_data(y0 >> 8);
    lcd_send_data(y0 & 0xFF);
    lcd_send_data(y1 >> 8);
    lcd_send_data(y1 & 0xFF);

    lcd_send_command(0x2C);     // 开始写入GRAM
}

// 显示RGB565图像（图像数据需为uint16_t数组）
static void lcd_show_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *img)
{
    lcd_set_window(x, y, x + width - 1, y + height - 1);

    LCD_RS_EN(1);
    LCD_CS_EN(1);

    // 批量发送数据（优化传输）
    for (uint32_t i = 0; i < width * height; i++) {
        uint16_t color = img[i];
        lcd_send_data(color >> 8);   // 发送高字节
        lcd_send_data(color & 0xFF); // 发送低字节
    }
    LCD_CS_EN(0);
}

static void lcd_fill_color(uint16_t color, uint32_t pixel_count)
{
    lcd_set_window(0, 0, SOC_WIDTH - 1, SOC_HEIGHT - 1); // 全屏范围

    LCD_RS_EN(1);
    LCD_CS_EN(1);

    for (uint32_t i = 0; i < pixel_count; i++) {
        // 发送颜色高字节和低字节
        lcd_send_data(color >> 8);   // 发送高8位（如0x00）
        lcd_send_data(color & 0xFF); // 发送低8位（如0x1F）
    }

    LCD_CS_EN(0);
}

static void lcd_select_area_fill_color(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color)
{
    /* 参数有效性校验 */
    if(x_start >= SOC_HEIGHT || y_start >= SOC_WIDTH || 
       x_end >= SOC_HEIGHT || y_end >= SOC_WIDTH || 
       x_start > x_end || y_start > y_end) return;

    /* 计算区域尺寸 */
    uint16_t width = x_end - x_start + 1;
    uint16_t height = y_end - y_start + 1;
    uint32_t total_pixels = (uint32_t)width * height;

    /* 设置显示窗口 */
    lcd_set_window(x_start, y_start, x_end, y_end);
    
    /* 启用总线传输 */
    LCD_RS_EN(1);
    LCD_CS_EN(1);

    /* 连续写入颜色数据 */
    for(uint32_t i = 0; i < total_pixels; i++) {
        lcd_send_data(color >> 8);    // 发送高字节
        lcd_send_data(color & 0xFF);  // 发送低字节
        // lcd_send_data_16bit(color);
    }

    /* 关闭总线 */
    LCD_CS_EN(0);
}

static void lcd_write_data(uint16_t *data, uint32_t len) 
{
    // 实现数据写入，具体根据屏幕驱动接口编写

    for (uint32_t i = 0; i < len; i++) {
        uint16_t color = data[i];
        lcd_send_data(color >> 8);   // 发送高8位（如0x00）
        lcd_send_data(color & 0xFF); // 发送低8位（如0x1F）
        // lcd_send_data_16bit(color);
    }
}

static void lcd_draw_char_12X6(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bcolor) 
{
    if (c < 0x20 || c > 0x7E) return; // 过滤非ASCII可打印字符
    uint8_t index = c - 0x20;         // 计算字符数组索引
    const uint8_t *char_data = asc2_1206[index];
    uint16_t buffer[6 * 12];          // 8列×16行像素缓冲区

    for (int row = 0; row < 12; row++) {
        uint8_t byte = char_data[row];
        for (int col = 0; col < 6; col++) {
            // 检查每个bit，高位在左
            if (byte & (0x80 >> col)) {
                buffer[row * 6 + col] = color; // 前景色
            } else {
                buffer[row * 6 + col] = bcolor; // 背景色（黑色）
            }
        }
    }

    // 设置显示窗口并写入数据
    lcd_set_window(x, y, x + 5, y + 11);
    lcd_write_data(buffer, sizeof(buffer)/sizeof(uint16_t));
}

static void lcd_draw_char_16X8(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bcolor) 
{
    if (c < 0x20 || c > 0x7E) return; // 过滤非ASCII可打印字符
    uint8_t index = c - 0x20;         // 计算字符数组索引
    const uint8_t *char_data = asc2_1608[index];
    uint16_t buffer[8 * 16];          // 8列×16行像素缓冲区

    for (int row = 0; row < 16; row++) {
        uint8_t byte = char_data[row];
        for (int col = 0; col < 8; col++) {
            // 检查每个bit，高位在左
            if (byte & (0x80 >> col)) {
                buffer[row * 8 + col] = color; // 前景色
            } else {
                buffer[row * 8 + col] = bcolor; // 背景色（黑色）
            }
        }
    }

    // 设置显示窗口并写入数据
    lcd_set_window(x, y, x + 7, y + 15);
    lcd_write_data(buffer, sizeof(buffer)/sizeof(uint16_t));
}

static void lcd_draw_char_24X12(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bcolor) 
{
    if (c < 0x20 || c > 0x7E) return; // 过滤非ASCII可打印字符
    uint8_t index = c - 0x20;         // 计算字符数组索引
    const uint8_t *char_data = asc2_2412[index];
    uint16_t buffer[12 * 24];          // 8列×16行像素缓冲区

    for (int row = 0; row < 24; row++) {
        uint16_t byte = (char_data[row * 2] << 8) + char_data[row * 2 + 1];
        for (int col = 0; col < 12; col++) {
            // 检查每个bit，高位在左
            if (byte & (0x8000 >> col)) {
                buffer[row * 12 + col] = color; // 前景色
            } else {
                buffer[row * 12 + col] = bcolor; // 背景色（黑色）
            }
        }
    }

    // 设置显示窗口并写入数据
    lcd_set_window(x, y, x + 11, y + 23);
    lcd_write_data(buffer, sizeof(buffer)/sizeof(uint16_t));
}

static void lcd_draw_char_32X16(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bcolor) 
{
    if (c < 0x20 || c > 0x7E) return; // 过滤非ASCII可打印字符
    uint8_t index = c - 0x20;         // 计算字符数组索引
    const uint8_t *char_data = asc2_3216[index];
    uint16_t buffer[16 * 32];          // 8列×16行像素缓冲区

    for (int row = 0; row < 32; row++) {
        uint16_t byte = (char_data[row * 2] << 8) + char_data[row * 2 + 1];
        for (int col = 0; col < 16; col++) {
            // 检查每个bit，高位在左
            if (byte & (0x8000 >> col)) {
                buffer[row * 16 + col] = color; // 前景色
            } else {
                buffer[row * 16 + col] = bcolor; // 背景色（黑色）
            }
        }
    }

    // 设置显示窗口并写入数据
    lcd_set_window(x, y, x + 15, y + 31);
    lcd_write_data(buffer, sizeof(buffer)/sizeof(uint16_t));
}

// 绘制字符串
static void lcd_draw_string(uint16_t x, uint16_t y, char *str, uint16_t color, uint16_t bcolor, FontType font_type) 
{
    while (*str) {
        if (font_type == FONT_12x6) {
            lcd_draw_char_12X6(x, y, *str, color, bcolor);
            x += 6; // 每个字符占6像素宽度
        } if (font_type == FONT_16x8) {
            lcd_draw_char_16X8(x, y, *str, color, bcolor);
            x += 8; // 每个字符占8像素宽度
        } else if (font_type == FONT_24x12) {
            lcd_draw_char_24X12(x, y, *str, color, bcolor);
            x += 12; // 每个字符占12像素宽度
        } else if (font_type == FONT_32x16) {
            lcd_draw_char_32X16(x, y, *str, color, bcolor);
            x += 16; // 每个字符占16像素宽度
        }
        str++;
    }
}

static void st7789X_init(void)
{
    lcd_reset();

    lcd_send_command(0x36);     
    lcd_send_data(0xB0);   

    lcd_send_command(0x3A);     
    lcd_send_data(0x05);   //06

    lcd_send_command(0xB2);     
    lcd_send_data(0x0C);   
    lcd_send_data(0x0C);   
    lcd_send_data(0x00);   
    lcd_send_data(0x33);   
    lcd_send_data(0x33);   

    lcd_send_command(0xB7);     
    lcd_send_data(0x35);   

    lcd_send_command(0xBB);     
    lcd_send_data(0x35);   

    lcd_send_command(0xC0);     
    lcd_send_data(0x2C);   

    lcd_send_command(0xC2);     
    lcd_send_data(0x01);   

    lcd_send_command(0xC3);     
    lcd_send_data(0x13);   

    lcd_send_command(0xC4);     
    lcd_send_data(0x20);   //VDV, 0x20:0v

    lcd_send_command(0xC6);     
    lcd_send_data(0x0F);   

    lcd_send_command(0xE7);     //2通道
    lcd_send_data(0x10);   

    lcd_send_command(0xD0);     
    lcd_send_data(0xA4);   
    lcd_send_data(0xA1);   

    lcd_send_command(0xD6);     
    lcd_send_data(0xA1);   //sleep in?,gate???GND

#if 1
    lcd_send_command(0xE0);     
    lcd_send_data(0xF0);   
    lcd_send_data(0x00);   
    lcd_send_data(0x04);   
    lcd_send_data(0x04);   
    lcd_send_data(0x04);   
    lcd_send_data(0x05);   
    lcd_send_data(0x29);   
    lcd_send_data(0x33);   
    lcd_send_data(0x3E);   
    lcd_send_data(0x38);   
    lcd_send_data(0x12);   
    lcd_send_data(0x12);   
    lcd_send_data(0x28);   
    lcd_send_data(0x30);   

    lcd_send_command(0xE1);     
    lcd_send_data(0xF0);   
    lcd_send_data(0x07);   
    lcd_send_data(0x0A);   
    lcd_send_data(0x0D);   
    lcd_send_data(0x0B);   
    lcd_send_data(0x07);   
    lcd_send_data(0x28);   
    lcd_send_data(0x33);   
    lcd_send_data(0x3E);   
    lcd_send_data(0x36);   
    lcd_send_data(0x14);   
    lcd_send_data(0x14);   
    lcd_send_data(0x29);   
    lcd_send_data(0x32);   
#else
    lcd_send_command(0xE0);
    lcd_send_data(0xD0);
    lcd_send_data(0x08);
    lcd_send_data(0x10);
    lcd_send_data(0x0D);
    lcd_send_data(0x0C);
    lcd_send_data(0x07);
    lcd_send_data(0x37);
    lcd_send_data(0x53);
    lcd_send_data(0x4C);
    lcd_send_data(0x39);
    lcd_send_data(0x15);
    lcd_send_data(0x15);
    lcd_send_data(0x2A);
    lcd_send_data(0x2D);

    lcd_send_command(0xE1);
    lcd_send_data(0xD0);
    lcd_send_data(0x0D);
    lcd_send_data(0x12);
    lcd_send_data(0x08);
    lcd_send_data(0x08);
    lcd_send_data(0x15);
    lcd_send_data(0x34);
    lcd_send_data(0x34);
    lcd_send_data(0x4A);
    lcd_send_data(0x36);
    lcd_send_data(0x12);
    lcd_send_data(0x13);
    lcd_send_data(0x2B);
    lcd_send_data(0x2F);
#endif

    lcd_send_command(0x21);  

    lcd_send_command(0x11);     

    /* 开启显示之前将屏幕设置为全黑，避免开启屏幕显示花屏 */
    lcd_fill_color(LCD_COLOR_BLACK, 240 * 320);

    Delay_Ms(200);  
    lcd_send_command(0x29); 

    lcd_send_command(0x2C);     

    lcd_send_command(0x36);     
    lcd_send_data(0xB0);   
}

void lcd_init(void)
{
    SPI1_Init();
    SPI1_DMA_Init();
    st7789X_init();

    lcd_show_image(LCD_HEIGHT_START + 31, LCD_WIDTH_START + 0, 177, 20, (uint16_t *)gImage_main_tool_name);
    lcd_show_image(LCD_HEIGHT_START + 10, LCD_WIDTH_START + 22, 60, 13, (uint16_t *)gImage_main_device_name);
    lcd_show_image(LCD_HEIGHT_START + 120, LCD_WIDTH_START + 22, 100, 20, (uint16_t *)gImage_main_press_delay);
    lcd_show_image(LCD_HEIGHT_START + 120, LCD_WIDTH_START + 78, 100, 20, (uint16_t *)gImage_main_lift_delay);
    lcd_show_image(LCD_HEIGHT_START + 10, LCD_WIDTH_START + 62, 80, 17, (uint16_t *)gImage_main_device_sta_disconnect);
    lcd_show_image(LCD_HEIGHT_START + 10, LCD_WIDTH_START + 86, 34, 14, (uint16_t *)gImage_main_test_mode);
    lcd_show_image(LCD_HEIGHT_START + 47, LCD_WIDTH_START + 84, 28, 14, (uint16_t *)gImage_main_square_wave);
    lcd_show_image(LCD_HEIGHT_START + 10, LCD_WIDTH_START + 103, 72, 22, (uint16_t *)gImage_main_test_sta_not_start);
}

void lcd_press_delay_time_set(uint32_t press_delay)
{
    lcd_select_area_fill_color(LCD_HEIGHT_START + 120, LCD_WIDTH_START + 50, LCD_HEIGHT_START + 120 + 120, LCD_WIDTH_START + 50 + 24, LCD_COLOR_BLACK);
    if (press_delay >= 99999) {
        press_delay = 99999;
    }
    if (press_delay > 9999) {
        press_delay_image_addr[0] = image_main_delay_num_addr[(press_delay / 10000)];
        press_delay_image_addr[1] = image_main_delay_num_addr[((press_delay % 10000) / 1000)];
        press_delay_image_addr[2] = image_main_delay_num_addr[(((press_delay % 10000) % 1000) / 100)];
        press_delay_image_addr[3] = image_main_delay_num_addr[((((press_delay % 10000) % 1000) % 100) / 10)];
        press_delay_image_addr[4] = image_main_delay_num_addr[((((press_delay % 10000) % 1000) % 100) % 10)];
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 0), LCD_WIDTH_START + 50, 14, 20, (uint16_t *)press_delay_image_addr[0]);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 1), LCD_WIDTH_START + 50, 14, 20, (uint16_t *)press_delay_image_addr[1]);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 2), LCD_WIDTH_START + 50, 6, 20, (uint16_t *)gImage_main_delay_point);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 2) + 6, LCD_WIDTH_START + 50, 14, 20, (uint16_t *)press_delay_image_addr[1]);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 3) + 6, LCD_WIDTH_START + 50, 14, 20, (uint16_t *)press_delay_image_addr[2]);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 4) + 6, LCD_WIDTH_START + 50, 14, 20, (uint16_t *)press_delay_image_addr[3]);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 5) + 6, LCD_WIDTH_START + 50, 21, 20, (uint16_t *)gImage_main_delay_m);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 5) + 6 + 21, LCD_WIDTH_START + 50, 12, 20, (uint16_t *)gImage_main_delay_s);
    } else {
        press_delay_image_addr[0] = image_main_delay_num_addr[((press_delay % 10000) / 1000)];
        press_delay_image_addr[1] = image_main_delay_num_addr[(((press_delay % 10000) % 1000) / 100)];
        press_delay_image_addr[2] = image_main_delay_num_addr[((((press_delay % 10000) % 1000) % 100) / 10)];
        press_delay_image_addr[3] = image_main_delay_num_addr[((((press_delay % 10000) % 1000) % 100) % 10)];
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 0), LCD_WIDTH_START + 50, 14, 20, (uint16_t *)press_delay_image_addr[0]);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 1), LCD_WIDTH_START + 50, 6, 20, (uint16_t *)gImage_main_delay_point);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 1) + 6, LCD_WIDTH_START + 50, 14, 20, (uint16_t *)press_delay_image_addr[1]);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 2) + 6, LCD_WIDTH_START + 50, 14, 20, (uint16_t *)press_delay_image_addr[2]);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 3) + 6, LCD_WIDTH_START + 50, 14, 20, (uint16_t *)press_delay_image_addr[3]);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 4) + 6, LCD_WIDTH_START + 50, 21, 20, (uint16_t *)gImage_main_delay_m);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 4) + 6 + 21, LCD_WIDTH_START + 50, 12, 20, (uint16_t *)gImage_main_delay_s);
    }
}

void lcd_lift_delay_time_set(uint32_t lift_delay)
{
    lcd_select_area_fill_color(LCD_HEIGHT_START + 120, LCD_WIDTH_START + 106, LCD_HEIGHT_START + 120 + 120, LCD_WIDTH_START + 106 + 24, LCD_COLOR_BLACK);
    if (lift_delay >= 99999) {
        lift_delay = 99999;
    }
    if (lift_delay > 9999) {
        lift_delay_image_addr[0] = image_main_delay_num_addr[(lift_delay / 10000)];
        lift_delay_image_addr[1] = image_main_delay_num_addr[((lift_delay % 10000) / 1000)];
        lift_delay_image_addr[2] = image_main_delay_num_addr[(((lift_delay % 10000) % 1000) / 100)];
        lift_delay_image_addr[3] = image_main_delay_num_addr[((((lift_delay % 10000) % 1000) % 100) / 10)];
        lift_delay_image_addr[4] = image_main_delay_num_addr[((((lift_delay % 10000) % 1000) % 100) % 10)];
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 0), LCD_WIDTH_START + 106, 14, 20, (uint16_t *)lift_delay_image_addr[0]);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 1), LCD_WIDTH_START + 106, 14, 20, (uint16_t *)lift_delay_image_addr[1]);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 2), LCD_WIDTH_START + 106, 6, 20, (uint16_t *)gImage_main_delay_point);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 2) + 6, LCD_WIDTH_START + 106, 14, 20, (uint16_t *)lift_delay_image_addr[1]);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 3) + 6, LCD_WIDTH_START + 106, 14, 20, (uint16_t *)lift_delay_image_addr[2]);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 4) + 6, LCD_WIDTH_START + 106, 14, 20, (uint16_t *)lift_delay_image_addr[3]);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 5) + 6, LCD_WIDTH_START + 106, 21, 20, (uint16_t *)gImage_main_delay_m);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 5) + 6 + 21, LCD_WIDTH_START + 106, 12, 20, (uint16_t *)gImage_main_delay_s);
    } else {
        lift_delay_image_addr[0] = image_main_delay_num_addr[((lift_delay % 10000) / 1000)];
        lift_delay_image_addr[1] = image_main_delay_num_addr[(((lift_delay % 10000) % 1000) / 100)];
        lift_delay_image_addr[2] = image_main_delay_num_addr[((((lift_delay % 10000) % 1000) % 100) / 10)];
        lift_delay_image_addr[3] = image_main_delay_num_addr[((((lift_delay % 10000) % 1000) % 100) % 10)];
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 0), LCD_WIDTH_START + 106, 14, 20, (uint16_t *)lift_delay_image_addr[0]);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 1), LCD_WIDTH_START + 106, 6, 20, (uint16_t *)gImage_main_delay_point);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 1) + 6, LCD_WIDTH_START + 106, 14, 20, (uint16_t *)lift_delay_image_addr[1]);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 2) + 6, LCD_WIDTH_START + 106, 14, 20, (uint16_t *)lift_delay_image_addr[2]);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 3) + 6, LCD_WIDTH_START + 106, 14, 20, (uint16_t *)lift_delay_image_addr[3]);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 4) + 6, LCD_WIDTH_START + 106, 21, 20, (uint16_t *)gImage_main_delay_m);
        lcd_show_image(LCD_HEIGHT_START + 120 + (14 * 4) + 6 + 21, LCD_WIDTH_START + 106, 12, 20, (uint16_t *)gImage_main_delay_s);
    }
}

void lcd_product_name_set(uint8_t product_name[])
{
    lcd_select_area_fill_color(LCD_HEIGHT_START + 10, LCD_WIDTH_START + 40, LCD_HEIGHT_START + 10 + 110, LCD_WIDTH_START + 40 + 14, LCD_COLOR_BLACK);
    for(uint8_t i = 0; i < 7; i++) {
        if ((product_name[i] & 0x60) == 0x60) {
            product_name_image_addr[i] = image_main_device_lowercase_addr[product_name[i] - 0x61];
        } else if ((product_name[i] & 0x40) == 0x40) {
            product_name_image_addr[i] = image_main_device_uppercase_addr[product_name[i] - 0x41];
        } else if ((product_name[i] & 0x30) == 0x30) {
            product_name_image_addr[i] = image_main_device_num_addr[product_name[i] - 0x30];
        } else {
            product_name_image_addr[i] = NULL;
        }
        if (i == 0) {
            product_name_image_x0[i] = (LCD_HEIGHT_START + 10 + (11 * i));
        } else {
            product_name_image_x0[i] = (product_name_image_x0[i - 1] + 11);
        } 
        if (product_name_image_addr[i] != NULL)
            lcd_show_image(product_name_image_x0[i], LCD_WIDTH_START + 40, 11, 14, (uint16_t *)product_name_image_addr[i]);
    }
}

void lcd_device_state_set(uint8_t sta)
{
    if (sta) {
        lcd_show_image(LCD_HEIGHT_START + 10, LCD_WIDTH_START + 62, 80, 17, (uint16_t *)gImage_main_device_sta_connect);
    } else {
        lcd_show_image(LCD_HEIGHT_START + 10, LCD_WIDTH_START + 62, 80, 17, (uint16_t *)gImage_main_device_sta_disconnect);
    }
}

void lcd_test_mode_set(uint8_t mode)
{
    lcd_select_area_fill_color(LCD_HEIGHT_START + 47, LCD_WIDTH_START + 84, LCD_HEIGHT_START + 47 + 56, LCD_WIDTH_START + 84 + 14, LCD_COLOR_BLACK);
    if (mode) {
        lcd_show_image(LCD_HEIGHT_START + 47, LCD_WIDTH_START + 84, 56, 14, (uint16_t *)gImage_main_simulate_click);
    } else {
        lcd_show_image(LCD_HEIGHT_START + 47, LCD_WIDTH_START + 84, 28, 14, (uint16_t *)gImage_main_square_wave);
    }
}

void lcd_test_state_set(uint8_t sta)
{
    if (sta == 1) {
        lcd_show_image(LCD_HEIGHT_START + 10, LCD_WIDTH_START + 103, 72, 22, (uint16_t *)gImage_main_test_sta_progress);
    } else if (sta == 2) {
        lcd_show_image(LCD_HEIGHT_START + 10, LCD_WIDTH_START + 103, 72, 22, (uint16_t *)gImage_main_test_sta_complete);
    } else {
        lcd_show_image(LCD_HEIGHT_START + 10, LCD_WIDTH_START + 103, 72, 22, (uint16_t *)gImage_main_test_sta_not_start);
    }
}

void lcd_test_data_anomaly_set(uint8_t sta)
{
    if (sta == 1) {
        lcd_show_image(LCD_HEIGHT_START + 120, LCD_WIDTH_START + 50, 98, 24, (uint16_t *)gImage_main_button_release);
    } else if (sta == 2) {
        lcd_show_image(LCD_HEIGHT_START + 120, LCD_WIDTH_START + 50, 98, 24, (uint16_t *)gImage_main_othe_reasons);
    }
    lcd_show_image(LCD_HEIGHT_START + 120, LCD_WIDTH_START + 106, 98, 24, (uint16_t *)gImage_main_data_anomaly);
}
