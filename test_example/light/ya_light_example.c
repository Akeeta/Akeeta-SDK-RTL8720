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
#include "cJSON.h"
#include "ya_app_main.h"
#include "ya_thing_data_api.h"
#include "ya_mode_api.h"
#include "ya_api_thing_uer_define.h"
#include "ya_light_io.h"

typedef enum{
	W_CONTROL = 0,
	C_CONTROL,
}ya_light_mode;

typedef enum{
	L_READ = 0,
	L_WORK,
	L_NIGHT,
	L_IDLE,
}ya_light_scene_mode;

typedef enum{
	NON_CONTROL = 0,
	WHITE_CONTROL,
	COLOR_CONTROL,
	SCENE_CONTROL,
}ya_light_control_mode;

#define MAX_W_COLOR    6500 //K
#define MIN_W_COLOR	   2000 //K

#define DEFAULT_W_COLOR	3000  //K

#define DEFAULT_W_COLOR_SCALE		(((DEFAULT_W_COLOR - MIN_W_COLOR) * 100)/(MAX_W_COLOR - MIN_W_COLOR))

#define DEFAULT_W_COLOR_READ_SCALE   (((4500- MIN_W_COLOR) * 100)/(MAX_W_COLOR - MIN_W_COLOR))
#define DEFAULT_W_COLOR_WORK_SCALE   (((3000- MIN_W_COLOR) * 100)/(MAX_W_COLOR - MIN_W_COLOR))
#define DEFAULT_W_COLOR_NIGHT_SCALE   (((6000- MIN_W_COLOR) * 100)/(MAX_W_COLOR - MIN_W_COLOR))
#define DEFAULT_W_COLOR_IDLE_SCALE   (((3500- MIN_W_COLOR) * 100)/(MAX_W_COLOR - MIN_W_COLOR))

ya_cloud_light_data_t ya_cloud_update_value;
ya_cloud_light_data_t ya_cloud_light_data_new;
ya_cloud_light_data_t ya_cloud_light_data_old_on_control;

void ya_cloud_light_set_to_config(ya_cloud_light_data_t *ya_cloud_light_data_config)
{
	ya_cloud_light_data_config->mode = W_CONTROL;
	ya_cloud_light_data_config->onoff = 1;
	ya_cloud_light_data_config->w_l = 50;
	ya_cloud_light_data_config->w_color = DEFAULT_W_COLOR_SCALE;
	ya_cloud_light_data_config->c_h = 0;
	ya_cloud_light_data_config->c_s = 0;
	ya_cloud_light_data_config->c_b = 0;
	ya_cloud_light_data_config->scene = -1;
}

void ya_cloud_light_set_to_idle(ya_cloud_light_data_t *ya_cloud_light_data_config)
{
	ya_cloud_light_data_config->mode = W_CONTROL;
	ya_cloud_light_data_config->onoff = 0;
	ya_cloud_light_data_config->w_l = 0;
	ya_cloud_light_data_config->w_color = 0;
	ya_cloud_light_data_config->c_h = 0;
	ya_cloud_light_data_config->c_s = 0;
	ya_cloud_light_data_config->c_b = 0;
	ya_cloud_light_data_config->scene = -1;
}

void ya_cloud_light_set_to_mode(ya_cloud_light_data_t *ya_cloud_light_data_config, ya_light_scene_mode mode)
{
	switch(mode)
	{
		case L_READ:
			ya_cloud_light_data_config->onoff = 1;
			ya_cloud_light_data_config->mode = W_CONTROL;
			ya_cloud_light_data_config->w_l = 75;
			ya_cloud_light_data_config->w_color = DEFAULT_W_COLOR_READ_SCALE;
			ya_cloud_light_data_config->c_h = 0;
			ya_cloud_light_data_config->c_s = 0;
			ya_cloud_light_data_config->c_b = 0;
			ya_cloud_light_data_config->scene = L_READ;
			break;

		case L_WORK:
			ya_cloud_light_data_config->onoff = 1;
			ya_cloud_light_data_config->mode = W_CONTROL;
			ya_cloud_light_data_config->w_l = 80;
			ya_cloud_light_data_config->w_color = DEFAULT_W_COLOR_WORK_SCALE;
			ya_cloud_light_data_config->c_h = 0;
			ya_cloud_light_data_config->c_s = 0;
			ya_cloud_light_data_config->c_b = 0;
			ya_cloud_light_data_config->scene = L_WORK;
			break;		

		case L_NIGHT:
			ya_cloud_light_data_config->onoff = 1;
			ya_cloud_light_data_config->mode = W_CONTROL;
			ya_cloud_light_data_config->w_l = 5;
			ya_cloud_light_data_config->w_color = DEFAULT_W_COLOR_NIGHT_SCALE;
			ya_cloud_light_data_config->c_h = 0;
			ya_cloud_light_data_config->c_s = 0;
			ya_cloud_light_data_config->c_b = 0;
			ya_cloud_light_data_config->scene = L_NIGHT;
			break;	

		case L_IDLE:
			ya_cloud_light_data_config->onoff = 1;
			ya_cloud_light_data_config->mode = W_CONTROL;
			ya_cloud_light_data_config->w_l = 50;
			ya_cloud_light_data_config->w_color = DEFAULT_W_COLOR_IDLE_SCALE;
			ya_cloud_light_data_config->c_h = 0;
			ya_cloud_light_data_config->c_s = 0;
			ya_cloud_light_data_config->c_b = 0;
			ya_cloud_light_data_config->scene = L_IDLE;
			break;

		default:
			break;
	}
}


void ya_cloud_light_set_to_mode_on(ya_cloud_light_data_t *ya_cloud_light_data_config)
{
	if(ya_cloud_light_data_old_on_control.onoff)
	{
		memcpy(&ya_cloud_light_data_new, &ya_cloud_light_data_old_on_control, sizeof(ya_cloud_light_data_t));
	}else
	{
		ya_cloud_light_set_to_config(&ya_cloud_light_data_old_on_control);
		ya_cloud_light_set_to_config(&ya_cloud_light_data_new);
	}

	ya_cloud_light_data_old_on_control.scene = -1;
	ya_cloud_light_data_new.scene = -1;
}

void ya_cloud_light_set_to_mode_off(ya_cloud_light_data_t *ya_cloud_light_data_config)
{
	ya_cloud_light_data_old_on_control.scene = -1;
	ya_cloud_light_data_new.scene = -1;
	ya_cloud_light_set_to_idle(ya_cloud_light_data_config);
}

void ya_cloud_update_old_control(ya_cloud_light_data_t *ya_cloud_light_data_config)
{
	memcpy(&ya_cloud_light_data_old_on_control, ya_cloud_light_data_config, sizeof(ya_cloud_light_data_t));
}

char *ya_int_to_string(uint32_t value)
{
	static char change_string[33];

	memset(change_string, 0, 33);
	snprintf(change_string, 33, "%d", value);
	return change_string;
}

int32_t ya_report_whole_key_value_formulate(void)
{
	int32_t ret = -1;
	cJSON *property = NULL;
	char *buf = NULL;
	
	property = cJSON_CreateObject();
	if(!property)
		return -1;

	memcpy(&ya_cloud_update_value, &ya_cloud_light_data_new, sizeof(ya_cloud_light_data_t));

	cJSON_AddStringToObject(property, "Switch", ya_int_to_string(ya_cloud_update_value.onoff));
	cJSON_AddStringToObject(property, "Luminance", ya_int_to_string(ya_cloud_update_value.w_l));
	cJSON_AddStringToObject(property, "ColorTemperature",  ya_int_to_string(ya_cloud_update_value.w_color));
	cJSON_AddStringToObject(property, "Hue",  ya_int_to_string(ya_cloud_update_value.c_h));
	cJSON_AddStringToObject(property, "Saturation",  ya_int_to_string(ya_cloud_update_value.c_s));
	cJSON_AddStringToObject(property, "Brightness",  ya_int_to_string(ya_cloud_update_value.c_b));
	cJSON_AddStringToObject(property, "LightScene",  ya_int_to_string(ya_cloud_update_value.scene));

	buf = cJSON_PrintUnformatted(property);	
	if(buf)
	{
		ret = ya_thing_report_to_cloud((uint8_t *)buf, strlen(buf));
		if(ret != 0)
			ya_printf(C_LOG_ERROR, "ya_thing_to_cloud_translate error\n");

		free(buf);
	}

	if(property)
		cJSON_Delete(property);

	return 0;
}

int32_t ya_report_delta_key_value_formulate(void)
{
	int32_t ret = -1;
	cJSON *property = NULL;
	char *buf = NULL;
	uint8_t flag = 0;

	property = cJSON_CreateObject();
	if(!property)
		return -1;

	if(ya_cloud_update_value.onoff !=  ya_cloud_light_data_new.onoff)
	{
		flag = 1;
		ya_cloud_update_value.onoff = ya_cloud_light_data_new.onoff;
		cJSON_AddStringToObject(property, "Switch", ya_int_to_string(ya_cloud_update_value.onoff));
	}
	if(ya_cloud_update_value.w_l !=  ya_cloud_light_data_new.w_l)
	{
		flag = 1;
		ya_cloud_update_value.w_l = ya_cloud_light_data_new.w_l;
		cJSON_AddStringToObject(property, "Luminance", ya_int_to_string(ya_cloud_update_value.w_l));
	}

	if(ya_cloud_update_value.w_color !=  ya_cloud_light_data_new.w_color)
	{
		flag = 1;
		ya_cloud_update_value.w_color = ya_cloud_light_data_new.w_color;
		cJSON_AddStringToObject(property, "ColorTemperature",  ya_int_to_string(ya_cloud_update_value.w_color));
	}
	
	if(ya_cloud_update_value.c_h !=  ya_cloud_light_data_new.c_h)
	{
		flag = 1;
		ya_cloud_update_value.c_h = ya_cloud_light_data_new.c_h;
		cJSON_AddStringToObject(property, "Hue",  ya_int_to_string(ya_cloud_update_value.c_h));
	}

	if(ya_cloud_update_value.c_s !=  ya_cloud_light_data_new.c_s)
	{
		flag = 1;
		ya_cloud_update_value.c_s  = ya_cloud_light_data_new.c_s;
		cJSON_AddStringToObject(property, "Saturation",  ya_int_to_string(ya_cloud_update_value.c_s));
	}
	
	if(ya_cloud_update_value.c_b !=  ya_cloud_light_data_new.c_b)
	{
		flag = 1;
		ya_cloud_update_value.c_b  = ya_cloud_light_data_new.c_b;
		cJSON_AddStringToObject(property, "Brightness",  ya_int_to_string(ya_cloud_update_value.c_b));
	}
	
	if(ya_cloud_update_value.scene !=  ya_cloud_light_data_new.scene)
	{
		flag = 1;
		ya_cloud_update_value.scene  = ya_cloud_light_data_new.scene;
		cJSON_AddStringToObject(property, "LightScene",  ya_int_to_string(ya_cloud_update_value.scene));
	}
	
	if(flag)
	{
		buf = cJSON_PrintUnformatted(property);	
		if(buf)
		{
			ret = ya_thing_report_to_cloud((uint8_t *)buf, strlen(buf));
			if(ret != 0)
				ya_printf(C_LOG_ERROR, "ya_thing_to_cloud_translate error\n");

			free(buf);
		}
	}
	
	if(property)
		cJSON_Delete(property);

	return 0;
}


void ya_set_light_para(ya_cloud_light_data_t *ya_ligth_para)
{
	ya_cloud_light_data_t ya_cloud_light_data;

	memcpy(&ya_cloud_light_data, ya_ligth_para, sizeof(ya_cloud_light_data_t));
	if(ya_ligth_para->mode == W_CONTROL)
	{
		ya_cloud_light_data.c_b = 0;
		ya_cloud_light_data.c_h = 0;
		ya_cloud_light_data.c_s = 0;
	}else
	{
		ya_cloud_light_data.w_l = 0;
		ya_cloud_light_data.w_color = 0;
	}

	ya_write_light_para(ya_cloud_light_data);
}

void ya_thing_handle_downlink_data(uint8_t *buf, uint16_t len)
{
	uint8_t enable_send = 0, get_switch_off = 0;
	ya_light_control_mode control_type = NON_CONTROL;
	cJSON *property = NULL, *json_key = NULL;
	uint32_t value = 0;

	property = cJSON_Parse((char *)buf);
	if(!property)
		goto err;

	json_key = cJSON_GetObjectItem(property, "Switch");
	if(json_key)
	{
		if(json_key->type !=cJSON_String)
			goto err;

		value = atoi(json_key->valuestring);
		if(value != 0)	
		{
			ya_cloud_light_data_new.onoff = 1;
			//use the old value
			ya_cloud_light_set_to_mode_on(&ya_cloud_light_data_new);
		}
		else
		{
			get_switch_off = 1;
			ya_cloud_light_data_new.onoff = 0;
			ya_cloud_light_set_to_mode_off(&ya_cloud_light_data_new);
		}
		enable_send = 1;
	}

	if(!get_switch_off)
	{
		if(control_type == WHITE_CONTROL || control_type == NON_CONTROL)
		{
			json_key = cJSON_GetObjectItem(property, "Luminance");
			if(json_key)
			{
				if(json_key->type !=cJSON_String)
					goto err;

				value = atoi(json_key->valuestring);
				ya_cloud_light_data_new.w_l = value;
				ya_cloud_light_data_new.mode = W_CONTROL;
				ya_cloud_light_data_new.onoff = 1;
				ya_cloud_light_data_new.scene = -1;

				enable_send = 1;
				control_type = WHITE_CONTROL;
			}

			json_key = cJSON_GetObjectItem(property, "ColorTemperature");
			if(json_key)
			{
				if(json_key->type !=cJSON_String)
					goto err;
			
				value = atoi(json_key->valuestring);
				ya_cloud_light_data_new.w_color = value;
				ya_cloud_light_data_new.mode = W_CONTROL;
				ya_cloud_light_data_new.onoff = 1;
				ya_cloud_light_data_new.scene = -1;

				enable_send = 1;
				control_type = WHITE_CONTROL;
			}

		}

		if(control_type == COLOR_CONTROL || control_type == NON_CONTROL)
		{
			json_key = cJSON_GetObjectItem(property, "Hue");
			if(json_key)
			{
				if(json_key->type !=cJSON_String)
					goto err;

				value = atoi(json_key->valuestring);
				ya_cloud_light_data_new.c_h = value;
				ya_cloud_light_data_new.onoff = 1;
				ya_cloud_light_data_new.mode = C_CONTROL;
				ya_cloud_light_data_new.scene = -1;

				enable_send = 1;
				control_type = COLOR_CONTROL;
			}

			json_key = cJSON_GetObjectItem(property, "Saturation");
			if(json_key)
			{
				if(json_key->type !=cJSON_String)
					goto err;

				value = atoi(json_key->valuestring);
				ya_cloud_light_data_new.c_s = value;
				ya_cloud_light_data_new.onoff = 1;
				ya_cloud_light_data_new.mode = C_CONTROL;
				ya_cloud_light_data_new.scene = -1;

				enable_send = 1;
				control_type = COLOR_CONTROL;
			}

			json_key = cJSON_GetObjectItem(property, "Brightness");
			if(json_key)
			{
				if(json_key->type !=cJSON_String)
					goto err;

				value = atoi(json_key->valuestring);
				ya_cloud_light_data_new.c_b = value;
				ya_cloud_light_data_new.onoff = 1;
				ya_cloud_light_data_new.mode = C_CONTROL;
				ya_cloud_light_data_new.scene = -1;

				enable_send = 1;
				control_type = COLOR_CONTROL;
			}
		}

		if(control_type == SCENE_CONTROL || control_type == NON_CONTROL)
		{

			json_key = cJSON_GetObjectItem(property, "LightScene");
			if(json_key)
			{
				if(json_key->type !=cJSON_String)
					goto err;

				value = atoi(json_key->valuestring);
				ya_cloud_light_set_to_mode(&ya_cloud_light_data_new, value);

				enable_send = 1;
				control_type = SCENE_CONTROL;
			}
		}
	}

	if(enable_send)
	{
		ya_report_delta_key_value_formulate();

		if(ya_cloud_light_data_new.onoff)
			ya_cloud_update_old_control(&ya_cloud_light_data_new);

		ya_set_light_para(&ya_cloud_light_data_new);
	}

	if(property)
		cJSON_Delete(property);
	
	err:

	if(property)
		cJSON_Delete(property);
}


uint32_t time_blink = 0;
Ya_Timer ya_aws_hardware_timer_blink;

void ya_thing_handle_cloud_status(ya_cloud_status_t ya_cloud_status)
{
	if(ya_cloud_status == YA_CLOUD_ONLINE)
		ya_report_whole_key_value_formulate();
}

void ya_thing_handle_router_status_callback(ya_router_status_t ya_app_router_status)
{
	uint32_t obj_blink_time = 0;
	if(ya_app_router_status == MODULE_SNIFFER)
	{
		obj_blink_time = 1500;
	}else if(ya_app_router_status == MODULE_AP)
	{
		obj_blink_time = 3000;
	}else if(ya_app_router_status == MODULE_IDLE)
	{
		ya_cloud_light_set_to_config(&ya_cloud_light_data_old_on_control);
		ya_cloud_light_set_to_config(&ya_cloud_light_data_new);
		ya_write_light_para(ya_cloud_light_data_new);
	}

	if(time_blink != obj_blink_time)
	{
		time_blink = obj_blink_time;
		ya_init_timer(&ya_aws_hardware_timer_blink);
		ya_countdown_ms(&ya_aws_hardware_timer_blink, time_blink);

		ya_cloud_light_set_to_config(&ya_cloud_light_data_new);
		ya_write_light_para(ya_cloud_light_data_new);
	}

	if(ya_app_router_status == MODULE_CONNECTING || ya_app_router_status == MODULE_IDLE ||ya_app_router_status == MODLUE_CONNECT)
	{
		time_blink = 0;
		ya_cloud_light_set_to_config(&ya_cloud_light_data_old_on_control);
		ya_cloud_light_set_to_config(&ya_cloud_light_data_new);
		ya_write_light_para(ya_cloud_light_data_new);
	}
}

void ya_thing_app_tick(void)
{
	if(time_blink)
	{		
		if(ya_has_timer_expired(&ya_aws_hardware_timer_blink) == C_OK)
		{
			ya_countdown_ms(&ya_aws_hardware_timer_blink, time_blink);
			if(ya_cloud_light_data_new.onoff)
				ya_cloud_light_set_to_mode_off(&ya_cloud_light_data_new);
			else
				ya_cloud_light_set_to_config(&ya_cloud_light_data_new);
			
			ya_write_light_para(ya_cloud_light_data_new);
		}	
	}
}

void ya_ligth_init(void)
{
	memset(&ya_cloud_light_data_new, 0, sizeof(ya_cloud_light_data_t));
	memset(&ya_cloud_light_data_old_on_control, 0, sizeof(ya_cloud_light_data_t));

	ya_cloud_light_set_to_mode_off(&ya_cloud_light_data_new);
	ya_init_light_io();	
}


ya_app_main_para_t test_light_main_para;

//test us akeeta
#if 1

#define TEST_VERSION	"rt.wcx.200.001"

#if 0
#define KEY1	"-----BEGIN CERTIFICATE-----\nMIIDWTCCAkGgAwIBAgIUTJ1nvE607/wr7KSW2ISbgQK9hAMwDQYJKoZIhvcNAQEL\nBQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\nSW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIwMDMzMDA2NDQw\nMloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\nZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMZ7sTaefCQNr0DPn5wv\nFB0OFADUi/tvpB99U/P1poaPEBKpHGCybzHgXSu327OMF+W6O8Zf6LJJnd90zpx8\n1buN5qeJiwHqaFosWXSanPReCdZKk5n+Geg1W7G+yWCHTbjrqGPxKRyu7lns9qeu\nREAGZTJ2cXoNNIFbzHCE/HZnbbL3cYUdLyILmrIU4fILj8hoVHaJg0eTz/cfCGWN\nMw5am8qklv8mrUNeHt6tqNn6Tubddgn7WsRfGdMfZSw7KhQJ/QfYfqSrNwYcT8OU\nmG7w6pXXRRPj0Muckk69/3nTDNxLFXUt6vbYoE3kRtqpFXf57e3GjGdA/ftv+fF+\naw0CAwEAAaNgMF4wHwYDVR0jBBgwFoAUVy/1ZNIznAffk90MreJL6LY40L8wHQYD\nVR0OBBYEFMyz39keQqf5Ms5Af2ThpweyjmvrMAwGA1UdEwEB/wQCMAAwDgYDVR0P\nAQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQAa2DpPkQzzrwg1Bt1RoclyndCG\n3tzqdhNiEbR6EGrutLrChvb3L15JFWfb3JFE/7kcztxrRRrxG4jOza59tL8rRfd8\nz5MHXfJ0wYwMxUxthY8IkDJPlXt6Iy6mUkW+9P939yvxBiVuWep/0Z9kamu2c5U7\ntNs5IXPp+TD5nODbecV3+Vl6FuOFbFBVASy/62jIrZQ5o7C9aY6+XR39apZ4DN/f\nYhzCgF8EONLCr2MuijnPgjJ1lA8fQQwUZuFL0VSV4R9ur0tNzcFs0YhYNwHGwdwy\nH6qL+rR4qNzeiCBCLWc0qwX1qrUCm1ICO0s5RcW6+p0WY44nCUxTJ8qFVlZb\n-----END CERTIFICATE-----\n"
#define KEY2	"-----BEGIN RSA PRIVATE KEY-----\nMIIEpAIBAAKCAQEAxnuxNp58JA2vQM+fnC8UHQ4UANSL+2+kH31T8/Wmho8QEqkc\nYLJvMeBdK7fbs4wX5bo7xl/oskmd33TOnHzVu43mp4mLAepoWixZdJqc9F4J1kqT\nmf4Z6DVbsb7JYIdNuOuoY/EpHK7uWez2p65EQAZlMnZxeg00gVvMcIT8dmdtsvdx\nhR0vIguashTh8guPyGhUdomDR5PP9x8IZY0zDlqbyqSW/yatQ14e3q2o2fpO5t12\nCftaxF8Z0x9lLDsqFAn9B9h+pKs3BhxPw5SYbvDqlddFE+PQy5ySTr3/edMM3EsV\ndS3q9tigTeRG2qkVd/nt7caMZ0D9+2/58X5rDQIDAQABAoIBAC/rANec33+cxiix\nd2kvfH2fPnkM11a2lzr6+N+2+KMXutumdDFsIqg58IRzeNkaA1L06SV1Vipk13Zx\nLshc4wTKt1rPO/4UZ2kNt/MMuMPOSDWx/3x3+i0zZJW+58MkJ0Fzs02dlT1Vhjmz\nmcDVs6h5YskSC0daFMj+LpcjpAe5d7pWrtRf+yxc8CFxxxuDKySA7Ct+g4rqPAqE\nmhd1bRihVTeWVNxhK+4sS7Iqirk7aMzOIIrRBOly9pSy961OcXbz1FiZfRG18/kH\nMQu0k3wXMLHGsCLcIF45+eNzwDZVTnqkNLBFEyFTtxcxegM1XHkv4LbE8Hm++NrH\nJ2ndWskCgYEA6ddQVQornLNhdf/DUTeOWY2b4Tbfdac7OcNrbPAALrmuXC8cu/2x\ngQAkmMpLKbiD3+GDJ+1h3ZWl6sz0kmZDzYfqHxjX8bSPFctWh0HxDXQiBylHAx4q\nTvvicfVq8UQvpsTw3dPMCgZvkg/BcZCcOkUojmHFrbPQ1yGFvfrwYfcCgYEA2Uqk\nFTbo6CX7dL+/3yXzxK8I40W0W/qqmpc7BiYt93NX2yWuz0elYmEOlYpM/sMhjYht\nmizBGh73UH1ji1mwD1ieZ+jjh2rVuDSa+EFYexSGAD2/olXUYNz9NPBrTG+7diSD\nCLupmSAxQp5uJdJ9eWXPwb/Arg2BQq9UPhUXGhsCgYBMeJVUuha5EBB3PkTpcnmi\n36Ryajd5ik7b/QRMg9ALd72FeAK1l6lMvnc5YC5kXmsB9AWoMQEoLaAswQnaNodF\n14TA1qcH7Dw+rFiHsxDZjXjzTpRieKLtYYIUMmXesKXEq/L5GCJJxQEHiw9/zALL\nK4KvzVmQDJXnRJcwIbgq0QKBgQC4gC9IsCIdFDsqC7LAqDoO9EyIpeQaAYKg1u5Q\nL1TyeqbLtnhzGjOskBrREOiFnWw8/+2Ibp3cLxTx7DX82b+rmCSQuGeR0T39NfWr\n+9hEUc3iIIw6K8S0JpB9RfLlqS20iBYW8Tl1gqcxGNMQHGVcEf3RVp8lauQpmjPe\nrwu5pwKBgQDi7I7jxj9IPNQz7UIK/wIgBYlThnh8i91ThUgu2IsBoVIhOxpIS9nA\npxoDEpJBGuZGM5dmjozRLeZsXKS59qIMoXO2+Qql7Hjjry2qIMDbartN6mOi0Qhm\nxJ+AzxGtAlFCqLbpwIdoK1pGRlPCIaeEW2V6RvhQiweHsvDXZKI8OA==\n-----END RSA PRIVATE KEY-----\n"
#define KEY3	"e57333524f71429e8b00f23900d36534"
#define KEY4	"YG_POWER_OUTLET"
#endif

void ya_light_example(void)
{
	ya_ligth_init();

//	ya_test_akeeta_us_with_obj_cert(KEY1, KEY2, KEY3, KEY4);
	
	memset(&test_light_main_para, 0, sizeof(ya_app_main_para_t));
	test_light_main_para.cloud_type = AKEETA_US;
	test_light_main_para.sniffer_timeout = 300;
	test_light_main_para.ap_timeout = 300;
	test_light_main_para.ya_init_mode = AP_MODE;
	test_light_main_para.enable_factory_uart = 1;
	strcpy(test_light_main_para.cur_version, TEST_VERSION);
	
	ya_start_app_main(test_light_main_para);
}

#else
//test cn akeeta

#define TEST_VERSION	"rt.wcx.100.001"

#define KEY1	"a1l6fAJ3eQJ"
#define KEY2	"d896e07800020123"
#define KEY3	"xB2b36kLX1tHx2yJr9joQ2LpdWy4KMN1"

void ya_light_example(void)
{
	ya_outlet_init();
	ya_test_akeeta_cn_with_obj_cert(KEY1, KEY2, KEY3);
	
	memset(&test_light_main_para, 0, sizeof(ya_app_main_para_t));
	test_light_main_para.cloud_type = AKEETA_CN;
	test_light_main_para.sniffer_timeout = 300;
	test_light_main_para.ap_timeout = 300;
	test_light_main_para.ya_init_mode = SNIFFER_MODE;
	test_light_main_para.enable_factory_uart = 1;
	strcpy(test_light_main_para.cur_version, TEST_VERSION);
	
	ya_start_app_main(test_light_main_para);
}
#endif


