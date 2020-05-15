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

#ifndef _YA_HAL_COMMON_H_
#define _YA_HAL_COMMON_H_

#include "FreeRTOS.h"
#include "task.h"
#include "string.h"
#include "stdlib.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

#include "ya_hal_os.h"
#include "ya_hal_flash.h"
#include "ya_hal_net.h"
#include "ya_hal_wdt.h"
#include "ya_hal_wlan.h"
#include "ya_hal_flash.h"
#include "ya_hal_timer.h"

#define MSG_QUEUE_LENGTH		16

typedef struct MSG_T
{
	uint16_t len;
	uint8_t *addr;
	uint8_t type;
	uint8_t subtype;
	uint8_t source;
}msg_t;

typedef enum{
	C_ERROR = -1,
	C_OK = 0,
}C_RESULT_T;


typedef enum{
	C_LOG_ERROR = 0,
	C_LOG_INFO,
	C_LOG_DEBUG,
}C_LOG_T;

#define G_LOG_LEVEL C_LOG_DEBUG

#define ya_printf(level, fmt, arg...)     \
	do {\
		if (level <= G_LOG_LEVEL) {\
			{\
				if (level <= C_LOG_ERROR) {\
					printf("[ERROR]: "fmt, ##arg);\
				} \
				else {\
					printf(""fmt, ##arg);\
				}\
			} \
		}\
	}while(0)

#endif



