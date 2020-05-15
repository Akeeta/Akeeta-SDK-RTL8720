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

/*
UART0 pin mappings used in this test demo:
            PA23(TX)
            PA18(RX)
*/

#include "FreeRTOS.h"
#include "serial_api.h"
#include "ya_hal_uart.h"
#include "uart_socket.h"
#include "sockets.h"

uart_socket_t *ya_uart_socket = NULL;

int32_t ya_hal_uart_open()
{
	uart_set_str uartset;
	
	uartset.BaudRate = 115200;
	uartset.number = 8;
	uartset.StopBits = 0;
	uartset.FlowControl = 0;
	uartset.parity = 0;
	strcpy(uartset.UartName, "uart0");

	ya_uart_socket = uart_open(&uartset);
	if(ya_uart_socket == NULL)
		return -1;

	return 0;
}


int32_t ya_hal_uart_write(uint8_t *buffer, uint32_t len)
{
	int32_t ret = -1;
	if(ya_uart_socket == NULL)
		return -1;

	ret = uart_write(ya_uart_socket, buffer, len);

	return ret;
}


int32_t ya_hal_uart_recv(uint8_t *buffer, uint32_t len, uint32_t timeout)
{
	fd_set readfds;
	struct timeval tv;
	int32_t ret = -1, num = 0;
	
	if(ya_uart_socket == NULL)
		return -1;
	
	FD_ZERO(&readfds);
	FD_SET(ya_uart_socket->fd, &readfds);
	tv.tv_sec = 0;
	tv.tv_usec = timeout * 1000;	

	ret = select(ya_uart_socket->fd + 1, &readfds, NULL, NULL, &tv);

	if(ret > 0)
	{
		if(FD_ISSET(ya_uart_socket->fd, &readfds))
		{
			num = uart_read(ya_uart_socket, buffer, len);
		}
	}

	return num;
}


