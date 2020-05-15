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

#ifndef _YA_THING_DATA_API_H_
#define _YA_THING_DATA_API_H_

/**
 * @brief This function is used to handle the translate from thing mode to cloud json.
 *
 * @return 0: sucess, -1: failed
 */
extern int32_t ya_thing_report_to_cloud(uint8_t *buf, uint16_t len);

/**
 * @brief This function is used to put data into queue, and wait the task to send it
 *
 * @return 0: sucess, -1: failed
 */

extern void ya_put_data_uart_into_queue(uint8_t *data, uint16_t data_len);


#endif

