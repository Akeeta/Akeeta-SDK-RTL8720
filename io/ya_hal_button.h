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


/*
reatlek 8710cx
Note
    1, Supported GPIO pins, PA_0, PA_1, PA_2, PA_3, PA_4, PA_7, PA_8, PA_9, PA_10, PA_11, PA_12, PA_13, PA_14, PA_15, PA_16, PA_17, PA_18, PA_19, PA_20, and PA_23.
    2, Please off JTAG/SWD, when using PA_0, PA_1, PA_2, PA_3, and PA_4.
    3, Please off log UART, when using PA_15, and PA_16.
    4, PA_7, PA_8, PA_9, PA_10, PA_11 and PA_12 only available on RTL8720CF.

*/


#ifndef _YA_HAL_BUTTON_H_
#define _YA_HAL_BUTTON_H_

#define BUTTON1_PIN        		PA_12
#define BUTTON1_DIRECTION		PIN_INPUT
#define BUTTON1_PULL_MODE		PullUp

#define BUTTON1_PUSH_VALUE		0
#define BUTTON1_RELEASE_VALUE	1


#define BUTTON_NUM				1		

#define YA_BUTTON_PARA_INIT	 {BUTTON1_PIN, BUTTON1_DIRECTION, BUTTON1_PULL_MODE,BUTTON1_PUSH_VALUE, NULL}

typedef enum
{
	BUTTON1 = 0,
}YA_BUTTON_NAME;

typedef void (*ya_button_push_time_func)(uint32_t button_push_ms);

#if (BUTTON_NUM > 0)

extern int ya_button_init(void);

extern void ya_set_button_callback(uint8_t index, ya_button_push_time_func ya_button_push_time_callback);

#endif

#endif




