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
#include "ya_hal_uart.h"
#include "ya_app_main.h"
#include "ya_thing_data_api.h"
#include "ya_mode_api.h"
#include "ya_api_thing_uer_define.h"
#include "cJSON.h"

ya_app_main_para_t test_uart_main_para;
ya_hal_os_thread_t uart_test = NULL;

#define TEST_VERSION	"rt.wcx.100.001"

uint8_t uart_buf[256];


void ya_thing_handle_downlink_data(uint8_t *buf, uint16_t len)
{
	// do your job here
	ya_printf(C_LOG_INFO, "cloud data: %s\n", buf);

	//may send to the uart	
	//Also, the user can use ya_hal_uart_write to send. 
	//ya_put_data_uart_into_queue(uart_buf, ret);
}

void ya_thing_handle_cloud_status(ya_cloud_status_t ya_cloud_status)
{
	if(ya_cloud_status == YA_CLOUD_ONLINE)
		ya_printf(C_LOG_INFO, "cloud online\n");
	else
		ya_printf(C_LOG_INFO, "cloud offline\n");
}

void ya_thing_handle_router_status_callback(ya_router_status_t ya_app_router_status)
{
	ya_printf(C_LOG_INFO, "router status: %d\n", ya_app_router_status);
}

int32_t ya_uart_report_packet_to_cloud_formulate(uint8_t value)
{
	int32_t ret = -1;
	cJSON *property = NULL;
	char *buf = NULL;
	
	property = cJSON_CreateObject();
	if(!property)
	{	
		return -1;
	}

	if(value)
		cJSON_AddStringToObject(property, "SwitchMultiple", "1");
	else
		cJSON_AddStringToObject(property, "SwitchMultiple", "0");

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

void ya_thing_app_tick(void)
{
	int ret = -1;
	ret = ya_hal_uart_recv(uart_buf, 256, 40);

	if(ret > 0)
	{		
		ya_printf(C_LOG_INFO, "get uart len: %d\n", ret);

		//may reponse to the uart 
		//Also, the user can use ya_hal_uart_write to send. 
		ya_put_data_uart_into_queue(uart_buf, ret);

		//may resonse to cloud, but need to change to json string
		ya_uart_report_packet_to_cloud_formulate(1);
	}		
}

void ya_user_uart_without_task_example(void)
{
	memset(&test_uart_main_para, 0, sizeof(ya_app_main_para_t));
	test_uart_main_para.cloud_type = AKEETA_US;
	test_uart_main_para.sniffer_timeout = 300;
	test_uart_main_para.ap_timeout = 300;
	test_uart_main_para.ya_init_mode = AP_MODE;	
	test_uart_main_para.enable_factory_uart = 0;
	strcpy(test_uart_main_para.cur_version, TEST_VERSION);
	
	ya_start_app_main(test_uart_main_para);
}



