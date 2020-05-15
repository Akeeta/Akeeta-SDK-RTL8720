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


#ifndef _YA_APP_MAIN_H_
#define _YA_APP_MAIN_H_

/* 
本头文件 有三个API接口。

关于证书

证书是与雅观云通信的凭证，如果需要连接雅观云成功，需要实现内置对应的证书到模组中。

当模组没有证书的时侯，可以使用以下方法设置证书：
1、使用enable_factory_uart = 1模式，然后使用雅观串口上位机给模组写入证书；适用于产检方式；

2、让模组进入热点模式，然后使用雅观局域网通信上位机给模组写入证书；适用于成品场合；

3、使用ya_test_akeeta_cn_with_obj_cert或者ya_test_akeeta_us_with_obj_cert 函数给模组设置一个固定的证书；
   每次调用启动函数ya_start_app_main之前，都需要事先调用一次ya_test_akeeta_cn_with_obj_cert或者ya_test_akeeta_us_with_obj_cert。

*/

typedef enum 
{
	/* the cloud is oversea */
	AKEETA_US = 0,
	
	/* the cloud is in China */
	AKEETA_CN,

	UNKNOWN_CLOUD_TYPE = 0xFF,
}CLOUD_T;

typedef enum{
	/* AP mode. The module will start AP/router with ssid is "Argrace_xxxxx". The "xxxx" is from the last two bytes of mac address.
	 Note this mode only used in factory state. When the module has obj router ssid and pwd, it will connect the router automatically*/
	AP_MODE = 0,

	/* sniffer mode. 
	Note this mode only used in factory state. When the module has obj router ssid and pwd, it will connect the router automatically*/
	SNIFFER_MODE,

	CONNECT_MODE,
	
	IDLE_MODE,
}ya_init_module_mode;

/* The product version length, the product version must meet the format "xx.xxx.xxx.xxx". */
#define   VERSION_LEN	14

typedef struct
{
	/* cloud  type */
	CLOUD_T 				cloud_type;
	
	/* intial mode when module is in factory state */
	ya_init_module_mode		ya_init_mode;
	
	/* sniffer timeout, we suggest 5 minutes or less. */
	uint32_t 				sniffer_timeout;
	
	/* AP timeout, we suggest 5 minutes or less. */
	uint32_t 				ap_timeout;
	
	/* The product version and it must meet the format "xx.xxx.xxx.xxx". The last three chars is from "000" to "999"  */
	char					cur_version[VERSION_LEN + 1];

	/* enable_factory_uart is used to enble the akeeta uart. akeeta uart is used to write and read licenses. 
	If the modlue has license and user has itself uart protocol, user should disable it. */
	bool					enable_factory_uart;
}ya_app_main_para_t;

/**
 * @brief This function is the one used to start the main app. 
 *
 * @return 0: sucess, -1: failed
 */
extern void ya_start_app_main(ya_app_main_para_t ya_app_main_para);

/**
 * @brief This function is used to set the user obj akeeta CN cert for test. If using the Akeeta cert tools to set the cert, there is no need to use this function.
 *
 * @return 0: sucess, -1: failed
 */
extern int ya_test_akeeta_cn_with_obj_cert(char *productkey, char *deviceid, char *secret);

/**
 * @brief This function is used to set the user obj akeeta US cert for test. If using the Akeeta cert tools to set the cert, there is no need to use this function.
 *
 * @return 0: sucess, -1: failed
 */
extern int ya_test_akeeta_us_with_obj_cert(char *certificateBuff, char *privateKeyBuff, char *client_id,  char *thing_type);



#endif

