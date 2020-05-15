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


#ifndef _YA_MODE_API_H_
#define _YA_MODE_API_H_

typedef enum
{
	YA_APP_IDLE	   				= 0x00,

	YA_APP_SNIFFER_START,
	YA_APP_SNIFFER,
	
	YA_APP_AP_START,
	YA_APP_AP_STATE,

	YA_APP_TO_CONNECT,
	YA_APP_CONNECTING,
	YA_APP_CONNECTED,
}ya_app_status_t;

/**
 * @brief This function is used to get the module state.
 *
 * @return 0: sucess, -1: failed
 */
extern ya_app_status_t ya_get_app_status(void);

/** 
 * @brief This function is used to reset the module to factory mode. The module will erase all flash data and reboot. 
 *
 * @return 0: sucess, -1: failed
 */
extern int32_t ya_reset_module_to_factory(void);

/**
 * @brief This function is used to set the module to toggle mode
 *
 * @param change_with_power_on: 1: when power on,  choose the toggle mode, 0: when normal running, choose the toggle mode and reboot.
 * @return 0: sucess, -1: failed
 */
extern int32_t ya_set_toggle_mode(uint8_t change_with_power_on);

/**
 * @brief This function is used to set the module to sniffer mode. The module will erase all flash data and reboot.
 *
 * @return 0: sucess, -1: failed
 */
extern int32_t ya_set_sniffer_mode(void);

/**
 * @brief This function is used to set the module to AP mode. The module will erase all flash data and reboot.
 *
 * @return 0: sucess, -1: failed
 */
extern int32_t ya_set_ap_mode(void);

#endif

