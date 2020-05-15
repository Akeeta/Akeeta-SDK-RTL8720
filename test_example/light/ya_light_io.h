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

#ifndef _YA_LIGHT_IO_H_
#define _YA_LIGHT_IO_H_

typedef struct _ya_ligth_para_
{
	unsigned  char  white_lumin;  //0~100 %
	unsigned  char color_Temp;   //0~100 %
	unsigned  short int hue;         //H    0~360  degree
	unsigned  short int saturation;   //S    0~100 %
	unsigned  short int brightness;   //V    0~100 %
} ya_ligth_para_t; 

typedef struct{
	uint8_t mode;
	uint8_t onoff;
	uint16_t w_l;
	uint16_t w_color;
	uint16_t c_h;
	uint16_t c_s;
	uint16_t c_b;
	int8_t scene;
}ya_cloud_light_data_t;

typedef enum
{
	YA_EVENT_CHANGE_WIFI_MODE	   				= 0x00,
}ya_light_event_t;

#define YA_LIGHT_FLASH_PARA_ADDR  (0x1d0000)

extern void ya_init_light_io(void);

extern void ya_write_light_para(ya_cloud_light_data_t ya_ligth_para);

#endif



