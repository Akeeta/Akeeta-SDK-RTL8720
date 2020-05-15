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
#include "sys_api.h"
#include "pwmout_api.h"
#include "ya_hal_pwm.h"

#if (PWM_NUM > 0)

typedef struct{
	PinName pin;
	int pwm_period;
	float pwm_percent;
}ya_pwm_para_t;

ya_pwm_para_t ya_pwm_para[PWM_NUM] = YA_PWM_PARA_INIT;

pwmout_t ya_pwm_obj[PWM_NUM];

void ya_hal_pwm_init(void)
{
	uint16_t index = 0;
	sys_jtag_off();

	for(index = 0; index < PWM_NUM; index++)
	{	
		pwmout_init(&ya_pwm_obj[index], ya_pwm_para[index].pin);
		pwmout_period_us(&ya_pwm_obj[index], ya_pwm_para[index].pwm_period);
		pwmout_write(&ya_pwm_obj[index], ya_pwm_para[index].pwm_percent);
	}
}

void ya_hal_pwm_write(uint8_t index, float percent)
{
	if(index < PWM_NUM)
		pwmout_write(&ya_pwm_obj[index], percent);
}

#endif

