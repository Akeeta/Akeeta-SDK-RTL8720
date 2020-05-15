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
#include "ya_hal_gpio.h"
#include "ya_hal_button.h"

#define YA_STRING_ON		"1"
#define YA_STRING_OFF		"0"

uint8_t switch_state = SWITCH1_OFF;
ya_router_status_t ya_router_status = MODULE_UNKNOWN;

extern int32_t ya_report_packet_formulate(char *value);

void ya_set_led_according_switch_value(void)
{
	if(switch_state == SWITCH1_ON)
	{
		ya_gpio_write(LED1, LED1_ON);
	}else
	{
		ya_gpio_write(LED1, LED1_OFF);
	}
}

void ya_button_callback_handle(uint32_t button_push_ms)
{
	msg_t msg;

	ya_printf(C_LOG_INFO, "button time: %d\n", button_push_ms);
	memset(&msg, 0, sizeof(msg_t));

	if(button_push_ms > (1000 * 10))
	{
		ya_reset_module_to_factory();
	}else if(button_push_ms > (1000 * 5))
	{
		ya_set_toggle_mode(0);
	}else
	{
		if(switch_state)
		{
			switch_state = SWITCH1_OFF;
			ya_gpio_write(SWITCH1, SWITCH1_OFF);

			if(ya_router_status == MODLUE_CONNECT)
				ya_report_packet_formulate(YA_STRING_OFF);
		}else
		{
			switch_state = SWITCH1_ON;
			ya_gpio_write(SWITCH1, SWITCH1_ON);

			if(ya_router_status == MODLUE_CONNECT)
				ya_report_packet_formulate(YA_STRING_ON);
		}

		if(ya_router_status != MODULE_AP && ya_router_status != MODULE_SNIFFER)
		{
			ya_set_led_according_switch_value();
		}
	}
}


int32_t ya_report_packet_formulate(char *value)
{
	int32_t ret = -1;
	cJSON *property = NULL;
	char *buf = NULL;
	
	property = cJSON_CreateObject();
	if(!property)
	{	
		return -1;
	}

	cJSON_AddStringToObject(property, "SwitchMultiple", value);

	buf = cJSON_PrintUnformatted(property);	
	if(buf)
	{
		ret = ya_thing_report_to_cloud((uint8_t *)buf, strlen(buf));
		if(ret != 0)
			ya_printf(C_LOG_ERROR, "ya_thing_report_to_cloud error\n");

		free(buf);
	}

	if(property)
		cJSON_Delete(property);

	return 0;
}


void ya_thing_handle_downlink_data(uint8_t *buf, uint16_t len)
{
	cJSON *property = NULL, *Switch_1 = NULL;
	uint32_t value = 0;

	property = cJSON_Parse((char *)buf);
	if(!property)
		goto err;

	Switch_1 = cJSON_GetObjectItem(property, "SwitchMultiple");
	if(!Switch_1)
		goto err;

	if(Switch_1->type !=cJSON_String)
		goto err;

	value = atoi(Switch_1->valuestring);
	if(value)
	{
		ya_gpio_write(SWITCH1, SWITCH1_ON);
		ya_gpio_write(LED1, LED1_ON);
		ya_report_packet_formulate(YA_STRING_ON);
	}
	else
	{
		ya_gpio_write(SWITCH1, SWITCH1_OFF);
		ya_gpio_write(LED1, LED1_OFF);
		ya_report_packet_formulate(YA_STRING_OFF);
	}

	if(property)
		cJSON_Delete(property);
	err:

	if(property)
		cJSON_Delete(property);
}


uint32_t time_blink = 0;
Ya_Timer ya_aws_hardware_timer_blink;

uint8_t led_state = 0;

void ya_thing_handle_cloud_status(ya_cloud_status_t ya_cloud_status)
{
	if(ya_cloud_status == YA_CLOUD_ONLINE)
	{
		if(switch_state)
		{
			ya_report_packet_formulate(YA_STRING_ON);
		}
		else
		{
			ya_report_packet_formulate(YA_STRING_OFF);
		}

	}
}

void ya_thing_handle_router_status_callback(ya_router_status_t ya_app_router_status)
{
	uint32_t obj_blink_time = 0;

	if(ya_router_status != ya_app_router_status)
		ya_router_status = ya_app_router_status;
	
	if(ya_app_router_status == MODULE_SNIFFER)
	{
		obj_blink_time = 1500;
	}else if(ya_app_router_status == MODULE_AP)
	{
		obj_blink_time = 3000;
	}	

	if(time_blink != obj_blink_time && obj_blink_time > 0)
	{
		time_blink = obj_blink_time;
		ya_init_timer(&ya_aws_hardware_timer_blink);
		ya_countdown_ms(&ya_aws_hardware_timer_blink, time_blink);
		
		led_state = 1;
		ya_gpio_write(LED1, LED1_ON);

	}

	if(ya_app_router_status == MODULE_CONNECTING || ya_app_router_status == MODULE_IDLE ||ya_app_router_status == MODLUE_CONNECT)
	{
		time_blink = 0;
		ya_set_led_according_switch_value();
	}
}

void ya_thing_app_tick(void)
{
	if(time_blink)
	{		
		if(ya_has_timer_expired(&ya_aws_hardware_timer_blink) == 0)
		{
			ya_countdown_ms(&ya_aws_hardware_timer_blink, time_blink);
			
			if(led_state)
			{
				led_state = 0;
				ya_gpio_write(LED1, LED1_OFF);
			}else
			{
				led_state = 1;
				ya_gpio_write(LED1, LED1_ON);
			}
		}	
	}
}

void ya_outlet_init(void)
{
	ya_gpio_init();

	//set led off
	led_state = 0;
	ya_gpio_write(LED1, LED1_OFF);

	//set switch off
	ya_gpio_write(SWITCH1, SWITCH1_OFF);
	
	ya_button_init();
	ya_set_button_callback(BUTTON1, ya_button_callback_handle);
}

ya_app_main_para_t test_outlet_main_para;

//test us akeeta
#if 1


#define TEST_VERSION	"rt.wcx.100.001"

#if 0

//Note: the following key is only example.

#define KEY1	"-----BEGIN CERTIFICATE-----\nMIIDWTCCAkGgAwIBAgIUTJ1nvE607/wr7KSW2ISbgQK9hAMwDQYJKoZIhvcNAQEL\nBQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\nSW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIwMDMzMDA2NDQw\nMloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\nZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMZ7sTaefCQNr0DPn5wv\nFB0OFADUi/tvpB99U/P1poaPEBKpHGCybzHgXSu327OMF+W6O8Zf6LJJnd90zpx8\n1buN5qeJiwHqaFosWXSanPReCdZKk5n+Geg1W7G+yWCHTbjrqGPxKRyu7lns9qeu\nREAGZTJ2cXoNNIFbzHCE/HZnbbL3cYUdLyILmrIU4fILj8hoVHaJg0eTz/cfCGWN\nMw5am8qklv8mrUNeHt6tqNn6Tubddgn7WsRfGdMfZSw7KhQJ/QfYfqSrNwYcT8OU\nmG7w6pXXRRPj0Muckk69/3nTDNxLFXUt6vbYoE3kRtqpFXf57e3GjGdA/ftv+fF+\naw0CAwEAAaNgMF4wHwYDVR0jBBgwFoAUVy/1ZNIznAffk90MreJL6LY40L8wHQYD\nVR0OBBYEFMyz39keQqf5Ms5Af2ThpweyjmvrMAwGA1UdEwEB/wQCMAAwDgYDVR0P\nAQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQAa2DpPkQzzrwg1Bt1RoclyndCG\n3tzqdhNiEbR6EGrutLrChvb3L15JFWfb3JFE/7kcztxrRRrxG4jOza59tL8rRfd8\nz5MHXfJ0wYwMxUxthY8IkDJPlXt6Iy6mUkW+9P939yvxBiVuWep/0Z9kamu2c5U7\ntNs5IXPp+TD5nODbecV3+Vl6FuOFbFBVASy/62jIrZQ5o7C9aY6+XR39apZ4DN/f\nYhzCgF8EONLCr2MuijnPgjJ1lA8fQQwUZuFL0VSV4R9ur0tNzcFs0YhYNwHGwdwy\nH6qL+rR4qNzeiCBCLWc0qwX1qrUCm1ICO0s5RcW6+p0WY44nCUxTJ8qFVlZb\n-----END CERTIFICATE-----\n"
#define KEY2	"-----BEGIN RSA PRIVATE KEY-----\nMIIEpAIBAAKCAQEAxnuxNp58JA2vQM+fnC8UHQ4UANSL+2+kH31T8/Wmho8QEqkc\nYLJvMeBdK7fbs4wX5bo7xl/oskmd33TOnHzVu43mp4mLAepoWixZdJqc9F4J1kqT\nmf4Z6DVbsb7JYIdNuOuoY/EpHK7uWez2p65EQAZlMnZxeg00gVvMcIT8dmdtsvdx\nhR0vIguashTh8guPyGhUdomDR5PP9x8IZY0zDlqbyqSW/yatQ14e3q2o2fpO5t12\nCftaxF8Z0x9lLDsqFAn9B9h+pKs3BhxPw5SYbvDqlddFE+PQy5ySTr3/edMM3EsV\ndS3q9tigTeRG2qkVd/nt7caMZ0D9+2/58X5rDQIDAQABAoIBAC/rANec33+cxiix\nd2kvfH2fPnkM11a2lzr6+N+2+KMXutumdDFsIqg58IRzeNkaA1L06SV1Vipk13Zx\nLshc4wTKt1rPO/4UZ2kNt/MMuMPOSDWx/3x3+i0zZJW+58MkJ0Fzs02dlT1Vhjmz\nmcDVs6h5YskSC0daFMj+LpcjpAe5d7pWrtRf+yxc8CFxxxuDKySA7Ct+g4rqPAqE\nmhd1bRihVTeWVNxhK+4sS7Iqirk7aMzOIIrRBOly9pSy961OcXbz1FiZfRG18/kH\nMQu0k3wXMLHGsCLcIF45+eNzwDZVTnqkNLBFEyFTtxcxegM1XHkv4LbE8Hm++NrH\nJ2ndWskCgYEA6ddQVQornLNhdf/DUTeOWY2b4Tbfdac7OcNrbPAALrmuXC8cu/2x\ngQAkmMpLKbiD3+GDJ+1h3ZWl6sz0kmZDzYfqHxjX8bSPFctWh0HxDXQiBylHAx4q\nTvvicfVq8UQvpsTw3dPMCgZvkg/BcZCcOkUojmHFrbPQ1yGFvfrwYfcCgYEA2Uqk\nFTbo6CX7dL+/3yXzxK8I40W0W/qqmpc7BiYt93NX2yWuz0elYmEOlYpM/sMhjYht\nmizBGh73UH1ji1mwD1ieZ+jjh2rVuDSa+EFYexSGAD2/olXUYNz9NPBrTG+7diSD\nCLupmSAxQp5uJdJ9eWXPwb/Arg2BQq9UPhUXGhsCgYBMeJVUuha5EBB3PkTpcnmi\n36Ryajd5ik7b/QRMg9ALd72FeAK1l6lMvnc5YC5kXmsB9AWoMQEoLaAswQnaNodF\n14TA1qcH7Dw+rFiHsxDZjXjzTpRieKLtYYIUMmXesKXEq/L5GCJJxQEHiw9/zALL\nK4KvzVmQDJXnRJcwIbgq0QKBgQC4gC9IsCIdFDsqC7LAqDoO9EyIpeQaAYKg1u5Q\nL1TyeqbLtnhzGjOskBrREOiFnWw8/+2Ibp3cLxTx7DX82b+rmCSQuGeR0T39NfWr\n+9hEUc3iIIw6K8S0JpB9RfLlqS20iBYW8Tl1gqcxGNMQHGVcEf3RVp8lauQpmjPe\nrwu5pwKBgQDi7I7jxj9IPNQz7UIK/wIgBYlThnh8i91ThUgu2IsBoVIhOxpIS9nA\npxoDEpJBGuZGM5dmjozRLeZsXKS59qIMoXO2+Qql7Hjjry2qIMDbartN6mOi0Qhm\nxJ+AzxGtAlFCqLbpwIdoK1pGRlPCIaeEW2V6RvhQiweHsvDXZKI8OA==\n-----END RSA PRIVATE KEY-----\n"
#define KEY3	"e57333524f71429e8b00f23900d36534"
#define KEY4	"YG_POWER_OUTLET"
#endif

void ya_outlet_example(void)
{
	ya_outlet_init();

//	ya_test_akeeta_us_with_obj_cert(KEY1, KEY2, KEY3, KEY4);
	
	memset(&test_outlet_main_para, 0, sizeof(ya_app_main_para_t));
	test_outlet_main_para.cloud_type = AKEETA_US;
	test_outlet_main_para.sniffer_timeout = 300;
	test_outlet_main_para.ap_timeout = 300;
	test_outlet_main_para.ya_init_mode = AP_MODE;
	test_outlet_main_para.enable_factory_uart = 1;
	strcpy(test_outlet_main_para.cur_version, TEST_VERSION);
	
	ya_start_app_main(test_outlet_main_para);
}

#else
//test cn akeeta

#define TEST_VERSION	"rt.wcx.100.001"

//Note: the following key is only example.

#define KEY1	"a1l6fAJ3eQJ"
#define KEY2	"d896e07800020123"
#define KEY3	"xB2b36kLX1tHx2yJr9joQ2LpdWy4KMN1"

void ya_outlet_example(void)
{
	ya_outlet_init();
	ya_test_akeeta_cn_with_obj_cert(KEY1, KEY2, KEY3);
	
	memset(&test_outlet_main_para, 0, sizeof(ya_app_main_para_t));
	test_outlet_main_para.cloud_type = AKEETA_CN;
	test_outlet_main_para.sniffer_timeout = 300;
	test_outlet_main_para.ap_timeout = 300;
	test_outlet_main_para.ya_init_mode = SNIFFER_MODE;
	test_outlet_main_para.enable_factory_uart = 1;
	strcpy(test_outlet_main_para.cur_version, TEST_VERSION);
	
	ya_start_app_main(test_outlet_main_para);
}
#endif

