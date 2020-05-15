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


#ifndef _YA_HAL_OS_H_
#define _YA_HAL_OS_H_

typedef xTaskHandle ya_hal_os_thread_t;

typedef xQueueHandle ya_hal_os_queue_t;

typedef xSemaphoreHandle ya_hal_os_mutex_t;


/**
 * @brief This function is the one used to printf the heap
 *
 */
extern void ya_printf_remain_heap(void);

/**
 * @brief This function is the one used to reboot the system
 *
 */
extern void ya_hal_sys_reboot(void);


/**
 * @brief This function is the one used to create the thread
 *
 * @param qhandle: the obj thread
 * @param name: thread name
 * @param main_func: the thread running function
 * @param arg: the running function para.
 * @param stack_depth: stack depth.
 * @param priority: the thread priority, 0 is the lowest priority in freeRTOS
 * @return 0: success, -1: fail
*/

extern int32_t ya_hal_os_thread_create(ya_hal_os_thread_t *thandle, 
											const char *name,
											void(*main_func)(void *arg),
											void *arg,
											int32_t stack_depth,
											int32_t priority);


/**
 * @brief This function is the one used to delete the thread
 *
 * @param thandle: the obj thread, if delete myselt, then the obj thread is NULL
 * @return 0: success, -1: fail
*/
extern int32_t ya_hal_os_thread_delete(ya_hal_os_thread_t *thandle);

/**
 * @brief This function is the one used to create the queue
 *
 * @param qhandle: the obj queue
 * @param name: queue name
 * @param msgsize: the sizeof message
 * @param queue_length: the msg num
 * @return 0: success, -1: fail
*/
extern int32_t ya_hal_os_queue_create(ya_hal_os_queue_t *qhandle, const char *name, int32_t msgsize, int32_t queue_length);

/**
 * @brief This function is the one used to deley the thread
 *
 */
extern void ya_hal_os_thread_sleep(int32_t mses);

/**
 * @brief This function is the one used to send msg to queue
 *
 * @param qhandle: the obj queue
 * @param msg: send message
 * @param msecs: if queue is full, then waitfor the time.
 * @return 0: success, -1: fail
 */
extern int32_t ya_hal_os_queue_send(ya_hal_os_queue_t *qhandle, const void *msg, uint32_t msecs);

/**
 * @brief This function is the one used to receive messages from queue
 *
 * @param qhandle: the obj queue
 * @param msg: receive message
 * @param msecs: waitting time
 * @return 0: success, -1: fail
 */
extern int32_t ya_hal_os_queue_recv(ya_hal_os_queue_t *qhandle, void *msg, uint32_t msecs);


/**
 * @brief This function is the one used to delete the obj queue
 *
 * @return 0: success, -1: fail
 */
extern int32_t ya_hal_os_queue_delete(ya_hal_os_queue_t *qhandle);

/**
 * @brief This function is the one used to get the system ticks
 *
 * @return ticks
 */
extern uint32_t ya_hal_os_tickcount_get(void);

/**
 * @brief This function is the one used to change the mili-seconds to ticks
 *
 * @param size: mili-seconds
 * @return ticks
 */
extern uint32_t ya_hal_os_msec_to_ticks(uint32_t msecs);


/**
 * @brief This function is the one used to change the system ticks to mili-seconds
 *
 * @return mili-seconds
 */
extern uint32_t ya_hal_os_ticks_to_msec(void);

/**
 * @brief This function is the one used to alloc the resource
 *
 * @param size: allocation size
 * @return allocation memory pointer
 */
extern void *ya_hal_os_memory_alloc(uint32_t size);

/**
 * @brief This function is the one used to free the memory
 *
 * @param size: free size
 */
extern void ya_hal_os_memory_free(void *pmem);

/**
 * @brief This function is the one used to calloc the resource
 *
 * @param size: callocation size
 * @return callocation memory pointer
 */
extern void* ya_hal_os_memory_calloc(uint32_t nelements, uint32_t elementSize);

#endif


