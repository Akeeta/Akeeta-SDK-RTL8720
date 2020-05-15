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

#include "FreeRTOS.h"
#include "gpio_api.h"
#include "ya_hal_gpio.h"

#if (GPIO_NUM > 0)
typedef struct{
	PinName 		pin;
	PinDirection 	pindiretion;
	PinMode 		mode;
}ya_gpio_para_t;

ya_gpio_para_t ya_gpio_para[GPIO_NUM] = YA_GPIO_PARA_INIT;

gpio_t ya_gpio[GPIO_NUM];

void ya_gpio_write(uint8_t index, int value)
{
	if(GPIO_NUM == 0 || index > GPIO_NUM)
	{
		return;
	}

	gpio_write(&ya_gpio[index], value);
}

int ya_gpio_read(uint8_t index)
{
	if(GPIO_NUM == 0 || index > GPIO_NUM)
	{
		return -1;
	}

	return gpio_read(&ya_gpio[index]);
}

void ya_gpio_init(void)
{
	uint8_t index = 0;

	sys_jtag_off();
	for(index = 0; index < GPIO_NUM; index++)
	{
		// Initial Push Button pin
		gpio_init(&ya_gpio[index], ya_gpio_para[index].pin);
		gpio_dir(&ya_gpio[index],  ya_gpio_para[index].pindiretion); 	
		gpio_mode(&ya_gpio[index], ya_gpio_para[index].mode);		
	}
}
#endif

