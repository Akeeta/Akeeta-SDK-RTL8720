/*
 * Copyright(c) 2018 - 2020 Yaguan Technology Ltd. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _YA_HAL_TIMER_H_
#define _YA_HAL_TIMER_H_

#include "stdint.h"
#include "FreeRTOS.h"

typedef enum {
    YA_TIMER_TYPE_MS   = 0x00,
    YA_TIMER_TYPE_S    = 0x01,
} ya_timer_type;

typedef struct  {
	uint32_t        time_out;
    TickType_t      init_tick;
    ya_timer_type      type;
}Ya_Timer;

//weishan
int32_t ya_has_timer_expired(Ya_Timer *Ya_Timer);

void ya_countdown_ms(Ya_Timer *Ya_Timer, uint32_t timeout);

void ya_countdown_sec(Ya_Timer *Ya_Timer, uint32_t timeout);

uint32_t ya_left_ms(Ya_Timer *Ya_Timer);

void ya_init_timer(Ya_Timer *Ya_Timer);

void ya_delay(uint32_t milliseconds);

#endif


