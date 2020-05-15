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

#ifndef _YA_HAL_WLAN_H_
#define _YA_HAL_WLAN_H_

typedef enum
{
	YA_HAL_EVT_LINK_LOST          = 0x01,

	YA_HAL_EVT_STA_CONNECT_SUCCESS,
	YA_HAL_EVT_STA_CONNECT_FAILED,

	YA_HAL_EVT_AP_START_SUCCESS,
	YA_HAL_EVT_AP_START_FAILED,
}ya_hal_wlan_event_t;

typedef enum
{
	YA_HAL_WLAN_STA_IDLE   				= 0x00,
	YA_HAL_WLAN_STA_CONNECTING 			= 0x01,
	YA_HAL_WLAN_STA_CONNECTED			= 0x02,
	YA_HAL_WLAN_STA_UNKNOWN_STATUS		= 0x03,
}ya_hal_wlan_sta_status_t;

typedef enum
{
	YA_HAL_WLAN_AP_IDLE   				= 0x00,
	YA_HAL_WLAN_AP_STARTING 			= 0x01,
	YA_HAL_WLAN_AP_RUNNING				= 0x02,
	YA_HAL_WLAN_AP_UNKNOWN_STATUS		= 0x03,
}ya_hal_wlan_ap_status_t;

typedef struct
{
	char ssid[33];
	char pwd[65];
	uint8_t bssid[6];
}ya_hal_wlan_ap_connect_param_t;

typedef struct
{
	char *ssid;
	uint8_t ssid_length;
	char *password;
	uint8_t password_length;
	uint8_t channel;
}ya_hal_wlan_ap_start_param_t;

typedef int (*ya_hal_wlan_event_handler_t)(ya_hal_wlan_event_t event, void *data);

/**
 * @brief This function is the one used to start wifi
 *
 * @param event_handler: callback the wifi event showed in ya_hal_wlan_event_t 
 * @return success: 0, faile: -1
 */
int32_t ya_hal_wlan_start(ya_hal_wlan_event_handler_t event_handler);

/**
 * @brief This function is the one used to close wifi.
 *
 * @return success: 0, faile: -1
 */
int32_t ya_hal_wlan_stop(void);

/**
 * @brief This function is the one used to disconnect the router in station mode
 *
 * @return success: 0, faile: -1
 */
int32_t ya_hal_wlan_disconnnect_wifi(void);

/**
 * @brief This function is the one used to set wifi to ap mode.
 *
 * @param ap_start_param: ap para. ssid, ssid_len, pwd, pwd_len, channel 
 * @return success: 0, faile: -1
 */
int32_t ya_hal_wlan_start_ap(ya_hal_wlan_ap_start_param_t *ap_start_param);

/**
 * @brief This function is the one used to stop ap and set the wifi to station mode
 *
 * @return success: 0, faile: -1
 */
int32_t ya_hal_wlan_stop_ap(void);

/**
 * @brief This function is the one used to set the wifi to station mode
 *
 * @return success: 0, faile: -1
 */
int32_t ya_hal_set_sta_mode(void);

/**
 * @brief This function is the one used to get the wifi module mac address
 *
 * @param mac_addr: output mac addr 
 * @return success: 0, faile: -1
 */
int32_t ya_hal_wlan_get_mac_address(uint8_t *mac_addr);


/**
 * @brief This function is the one used to get the wifi module mac address
 *
 * @param mac_addr: input mac addr 
 * @return success: 0, faile: -1
 */
int32_t ya_hal_wlan_set_mac_address(uint8_t *mac_addr);

/**
 * @brief This function is the one used to get the wifi ip address in station mode.
 *
 * @param ipv4_addr: output ipv4 addr array
 * @return success: 0, faile: -1
 */
int32_t ya_hal_wlan_get_ip_address(uint32_t *ipv4_addr);

/**
 * @brief This function is the one used to get the wifi ip address in station mode.
 *
 * @param ip: output ip string
 * @param len: input string len buffer
 * @return success: 0, faile: -1
 */
int32_t ya_hal_wlan_get_ip_address_with_string(uint8_t *ip, uint8_t len);

/**
 * @brief This function is the one used to get the router gateway ip address in station mode.
 *
 * @param gate_addr: output gateway addr array
 * @return success: 0, faile: -1
 */
int32_t ya_hal_wlan_get_gateway_ip_address(uint32_t *gate_addr);

/**
 * @brief This function is the one used to get rssi when the sta has connected to router
 *
 * @param rssi: output rssi
 * @return success: 0, faile: -1
 */
int32_t ya_hal_wlan_get_rssi(int32_t *rssi);


/**
 * @brief This function is the one used to get the current channel in sta mode.
 *
 * @param channel: output channel
 * @return success: 0, faile: -1
 */
int32_t ya_hal_wlan_get_channel(uint32_t *channel);

/**
 * @brief This function is the one used to set the channel in sta mode
 *
 * @param channel: input obj channel
 * @return success: 0, faile: -1
 */
int32_t ya_hal_wlan_set_channel(uint32_t channel);

/**
 * @brief This function is the one used to connect to router.
 *
 * @param connect_param: obj connect param
 * @return success: 0, faile: -1
 */
int32_t ya_hal_wlan_connect_ap(ya_hal_wlan_ap_connect_param_t *connect_param);


#endif

