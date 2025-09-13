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

#ifndef __TIM_H
#define __TIM_H

#include "ch32v30x_conf.h"

extern volatile uint32_t sys_timer_tick;
extern volatile uint32_t tim4_tick;

void TIM4_Init( uint16_t arr, uint16_t psc );
void TIM2_Init( uint16_t arr, uint16_t psc );
uint32_t get_sys_time_ms(void);
uint32_t get_sys_dt_ms(uint32_t time_ms);
uint32_t get_tim4_10us(void);
uint32_t get_tim4_dt_10us(uint32_t time_10us);

#endif