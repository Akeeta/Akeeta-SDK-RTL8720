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

#ifndef _YA_API_THING_USER_DEFINE_H_
#define _YA_API_THING_USER_DEFINE_H_

/**
The following funciton is realized by the user according to the thing table.
**/

/**
 * @brief This function is the one used to handle downlink contorl by cloud
 * @param buf: json string
 * @param len: json string len
 */
extern void ya_thing_handle_downlink_data(uint8_t *buf, uint16_t len);


/**
 * @brief This function is used to handle the cloud event and is defined by the user.
 *
 */
 typedef enum{
	YA_CLOUD_OFFLINE = 0,
	YA_CLOUD_ONLINE,
 }ya_cloud_status_t;	
extern void ya_thing_handle_cloud_status(ya_cloud_status_t ya_cloud_status);

/**
 * @brief This function is used to handle the router event and and is defined by the user.
 *
 */
typedef enum{
	MODULE_AP = 0,
	MODULE_SNIFFER,
	MODULE_IDLE,
	MODULE_CONNECTING,
	MODLUE_CONNECT,

	MODULE_UNKNOWN = 0xFF,
}ya_router_status_t;
	
typedef void (*ya_thing_handle_router_status)(ya_router_status_t ya_app_router_status);

extern void ya_thing_handle_router_status_callback(ya_router_status_t ya_app_router_status);

/**
 * Whenever app application tick is called, this callback will be called at the end of the app tick execution.
 *
 */
extern void ya_thing_app_tick(void);

#endif

