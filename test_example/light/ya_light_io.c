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

#include "ya_common.h"
#include "ya_light_io.h"
#include "hls_rgb.h"
#include "ya_hal_pwm.h"

void ya_pwm_led_write(unsigned char r,unsigned char g,unsigned char b,unsigned char c,unsigned char w)
{
	ya_hal_pwm_write(PWM_R,(float)(r/255.0));
	ya_hal_pwm_write(PWM_G,(float)(g/255.0));
	ya_hal_pwm_write(PWM_B,(float)(b/255.0));
	
	ya_hal_pwm_write(PWM_COOL,(float)(c/255.0));
	ya_hal_pwm_write(PWM_WARM,(float)(w/255.0));
}

void ya_write_light_para(ya_cloud_light_data_t ya_ligth_para)
{
	printf("set light para: %d, %d, %d, %d, %d\n", ya_ligth_para.w_l, ya_ligth_para.w_color,  ya_ligth_para.c_h,ya_ligth_para.c_s,ya_ligth_para.c_b);
	unsigned char r,g,b;
	unsigned char c,w;

	HSVtoRGB(&r,&g,&b,ya_ligth_para.c_h,ya_ligth_para.c_s,ya_ligth_para.c_b);	
	printf("rgb r=%d,g=%d,b=%d\n",r,g,b);

	c=100- ya_ligth_para.w_color;
	w=ya_ligth_para.w_color;

	c=c*(ya_ligth_para.w_l)*2.55/100;
	w=w*(ya_ligth_para.w_l)*2.55/100;  

	printf("cool=%d, warm=%d\n",c,w);
	ya_pwm_led_write(r,g,b,c,w);
}

void ya_light_read_power_on_flag(void)
{
	uint8_t buf[10];
	int32_t ret = -1;

	ret =  ya_hal_flash_read(YA_LIGHT_FLASH_PARA_ADDR, buf, 1);

	if(buf[0] == 0xFF)
		buf[0] = 0x00;

	buf[0]++;

	if(buf[0] == 4)
		ya_set_toggle_mode(1);;

	ret = ya_hal_flash_write(YA_LIGHT_FLASH_PARA_ADDR, buf, 1);
	
	if(ret == 0)
		ya_printf(C_LOG_INFO,"power flag: %d\n", buf[0]);		

}

void ya_light_clear_power_on_flag(void)
{
	int32_t ret = -1;
	uint8_t buf[10];

	buf[0] = 0;
	ret = ya_hal_flash_write(YA_LIGHT_FLASH_PARA_ADDR, buf, 1);

	if(ret == 0)
		ya_printf(C_LOG_INFO,"ya_light_clear_power_on_flag ok\n");		
}

void ya_clear_power_on_flag_callback(TimerHandle_t xTimer)
{
	ya_light_clear_power_on_flag();
}

void ya_init_light_io(void)
{
	TimerHandle_t xTimer;
	xTimer = xTimerCreate("power_on_clear_flag", ya_hal_os_msec_to_ticks(5000), pdFALSE, (void * )0, ya_clear_power_on_flag_callback);
	if( xTimer != NULL )
	{
		xTimerStart(xTimer, 0);
	}

	ya_light_read_power_on_flag();	
	ya_hal_pwm_init();
}

