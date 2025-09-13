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

#include "tim.h"

volatile uint32_t sys_timer_tick = 0;
volatile uint32_t tim4_tick = 0;

void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

extern uint8_t  square_wave_flag;
extern uint16_t square_wave_cnt;
extern uint8_t  square_single_press_flag;
extern uint8_t  square_single_release_flag;
extern uint32_t square_single_trigger_cnt;
extern uint32_t square_single_release_cnt;
extern uint16_t square_trigger_delay_time_buf[100];
extern uint16_t square_release_delay_time_buf[100];

extern uint8_t  triangle_wave_flag;
extern uint32_t triangle_wave_cnt;
extern uint8_t  triangle_wave_press_flag;
extern uint8_t  triangle_wave_release_flag;
extern uint32_t triangle_single_trigger_cnt;
extern uint32_t triangle_single_release_cnt;
extern uint16_t triangle_trigger_delay_time_buf[100];
extern uint16_t triangle_release_delay_time_buf[100];

extern uint8_t  simulation_curve_flag;
extern uint32_t simulation_curve_cnt;
extern uint8_t  simulation_curve_press_flag;
extern uint8_t  simulation_curve_release_flag;
extern uint32_t simulation_curve_single_trigger_cnt;
extern uint32_t simulation_curve_single_release_cnt;
extern uint16_t simulation_curve_trigger_delay_time_buf[100];
extern uint16_t simulation_curve_release_delay_time_buf[100];

/*********************************************************************
 * @fn      TIM2_Init
 *
 * @brief   Initialize timer2.
 *
 * @param   arr - The specific period value.
 *          psc - The specifies prescaler value.
 *
 * @return  none
 */
void TIM2_Init( uint16_t arr, uint16_t psc )
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = { 0 };
    NVIC_InitTypeDef NVIC_InitStructure = { 0 };

    /* Enable timer2 clock */
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );

    /* Initialize timer2 */
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM2, &TIM_TimeBaseStructure );

    /* Enable updating timer2 interrupt */
    TIM_ITConfig( TIM2, TIM_IT_Update, ENABLE );

    /* Configure timer2 interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );

    /* Enable timer2 */
    TIM_Cmd( TIM2, ENABLE );

    /* Enable timer2 interrupt */
    NVIC_EnableIRQ( TIM2_IRQn );
}

/*********************************************************************
 * @fn      TIM2_IRQHandler
 *
 * @brief   This function handles TIM2 global interrupt request.
 *
 * @return  none
 */
void TIM2_IRQHandler( void )
{
    if( TIM_GetITStatus( TIM2, TIM_IT_Update ) != RESET )
    {
        /* Clear interrupt flag */
        TIM_ClearITPendingBit( TIM2, TIM_IT_Update );
        sys_timer_tick++;
    }
}

uint32_t get_sys_time_ms(void)
{
    return sys_timer_tick;
}

uint32_t get_sys_dt_ms(uint32_t time_ms)
{
    if (sys_timer_tick >= time_ms)
        return sys_timer_tick - time_ms; // 这个是一般情况
    else
        return 0xFFFFFFFF - time_ms + sys_timer_tick; // 这个是溢出处理
}

/*********************************************************************
 * @fn      TIM4_Init
 *
 * @brief   Initialize timer4.
 *
 * @param   arr - The specific period value.
 *          psc - The specifies prescaler value.
 *
 * @return  none
 */
void TIM4_Init( uint16_t arr, uint16_t psc )
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = { 0 };
    NVIC_InitTypeDef NVIC_InitStructure = { 0 };

    /* Enable timer4 clock */
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE );

    /* Initialize timer4 */
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM4, &TIM_TimeBaseStructure );

    /* Enable updating timer4 interrupt */
    TIM_ITConfig( TIM4, TIM_IT_Update, ENABLE );

    /* Configure timer4 interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );

    /* Enable timer4 */
    TIM_Cmd( TIM4, ENABLE );

    /* Enable timer4 interrupt */
    NVIC_EnableIRQ( TIM4_IRQn );
}

/*********************************************************************
 * @fn      TIM4_IRQHandler
 *
 * @brief   This function handles TIM4 global interrupt request.
 *
 * @return  none
 */
void TIM4_IRQHandler( void )
{
    if( TIM_GetITStatus( TIM4, TIM_IT_Update ) != RESET )
    {
        /* Clear interrupt flag */
        TIM_ClearITPendingBit( TIM4, TIM_IT_Update );
        tim4_tick++;
        if (square_wave_flag) {
            if (square_single_press_flag) {
                ++square_single_trigger_cnt;
            } else {
                if (square_wave_cnt > 0)
                    square_trigger_delay_time_buf[square_wave_cnt - 1] = (square_single_trigger_cnt * 10);
            }
            if (square_single_release_flag) {
                ++square_single_release_cnt;
            } else {
                if (square_wave_cnt > 0)
                    square_release_delay_time_buf[square_wave_cnt - 1] = (square_single_release_cnt * 10);
            }
        }
        if (triangle_wave_flag) {
            if (triangle_wave_press_flag) {
                ++triangle_single_trigger_cnt;
            } else {
                triangle_trigger_delay_time_buf[triangle_wave_cnt] = (triangle_single_trigger_cnt * 10);
            }
            if (triangle_wave_release_flag) {
                ++triangle_single_release_cnt;
            } else {
                triangle_release_delay_time_buf[triangle_wave_cnt] = (triangle_single_release_cnt * 10);
            }
        }
        if (simulation_curve_flag) {
            if (simulation_curve_press_flag) {
                ++simulation_curve_single_trigger_cnt;
            } else {
                simulation_curve_trigger_delay_time_buf[simulation_curve_cnt] = (simulation_curve_single_trigger_cnt * 10);
            }
            if (simulation_curve_release_flag) {
                ++simulation_curve_single_release_cnt;
            } else {
                simulation_curve_release_delay_time_buf[simulation_curve_cnt] = (simulation_curve_single_release_cnt * 10);
            }
        }
    }
}

uint32_t get_tim4_10us(void)
{
    return tim4_tick;
}

uint32_t get_tim4_dt_10us(uint32_t time_10us)
{
    if (tim4_tick >= time_10us)
        return tim4_tick - time_10us; // 这个是一般情况
    else
        return 0xFFFFFFFF - time_10us + tim4_tick; // 这个是溢出处理
}