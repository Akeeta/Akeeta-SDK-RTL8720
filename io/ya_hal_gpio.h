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
realtek 8710cx
Note
    1, Supported GPIO pins, PA_0, PA_1, PA_2, PA_3, PA_4, PA_7, PA_8, PA_9, PA_10, PA_11, PA_12, PA_13, PA_14, PA_15, PA_16, PA_17, PA_18, PA_19, PA_20, and PA_23.
    2, Please off JTAG/SWD, when using PA_0, PA_1, PA_2, PA_3, and PA_4.
    3, Please off log UART, when using PA_15, and PA_16.
    4, PA_7, PA_8, PA_9, PA_10, PA_11 and PA_12 only available on RTL8720CF.

*/


#ifndef _YA_HAL_GPIO_H_
#define _YA_HAL_GPIO_H_

#define GPIO1_PIN        	PA_5
#define GPIO1_DIRECTION		PIN_OUTPUT
#define GPIO1_PULL_MODE		PullNone

#define GPIO2_PIN        	PA_22
#define GPIO2_DIRECTION		PIN_OUTPUT
#define GPIO2_PULL_MODE		PullNone

#define GPIO_NUM			2

#define YA_GPIO_PARA_INIT	 {{GPIO1_PIN, GPIO1_DIRECTION, GPIO1_PULL_MODE},\
							 {GPIO2_PIN, GPIO2_DIRECTION, GPIO2_PULL_MODE}}

typedef enum
{
	LED1 = 0,
	SWITCH1,
}YA_GPIO_NAME;

#define LED1_ON			0
#define LED1_OFF		1

#define SWITCH1_ON		1
#define SWITCH1_OFF 	0


#if (GPIO_NUM > 0)

extern void ya_gpio_write(uint8_t index, int value);

extern int ya_gpio_read(uint8_t index);

extern void ya_gpio_init(void);

#endif

#endif

