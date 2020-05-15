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
*Note 8710cx
*   1, Supported PWM, PWM_0 to PWM_7.
*        PWM_0, PA_0, PA_11, PA_20
*        PWM_1, PA_1, PA_12
*        PWM_2, PA_2, PA_14
*        PWM_3, PA_3, PA_15
*        PWM_4, PA_4, PA_16
*        PWM_5, PA_17
*        PWM_6, PA_18
*        PWM_7, PA_13, PA_19, PA_23
*    2, Please off JTAG/SWD, when using PA_0, PA_1, PA_2, PA_3, and PA_4.
*    3, Please off log UART, when using PA_15, and PA_16.
*/

#ifndef _YA_HAL_PWM_H_
#define _YA_HAL_PWM_H_

#define PWM_NUM					5

#define PWM1_PIN_NAME           PA_20
#define PWM1_PERIOD_US			1000
#define PWM1_PERCENT			0

#define PWM2_PIN_NAME           PA_2
#define PWM2_PERIOD_US			1000
#define PWM2_PERCENT			0

#define PWM3_PIN_NAME           PA_1
#define PWM3_PERIOD_US			1000
#define PWM3_PERCENT			0

#define PWM4_PIN_NAME           PA_23
#define PWM4_PERIOD_US			1000
#define PWM4_PERCENT			0

#define PWM5_PIN_NAME           PA_3
#define PWM5_PERIOD_US			1000
#define PWM5_PERCENT			0

#define YA_PWM_PARA_INIT	 {{PWM1_PIN_NAME, PWM1_PERIOD_US, PWM1_PERCENT},\
							 {PWM2_PIN_NAME, PWM2_PERIOD_US, PWM2_PERCENT},\
							 {PWM3_PIN_NAME, PWM3_PERIOD_US, PWM3_PERCENT},\
							 {PWM4_PIN_NAME, PWM4_PERIOD_US, PWM4_PERCENT},\
							 {PWM5_PIN_NAME, PWM5_PERIOD_US, PWM5_PERCENT}}

typedef enum
{
	PWM_R = 0,
	PWM_G,
	PWM_B,

	PWM_COOL,
	PWM_WARM,
}YA_PWM_NAME;


#if (PWM_NUM > 0)

extern void ya_hal_pwm_init(void);

extern void ya_hal_pwm_write(uint8_t index, float percent);

#endif

#endif

