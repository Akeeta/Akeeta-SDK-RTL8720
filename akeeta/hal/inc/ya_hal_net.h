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

#ifndef _YA_HAL_NET_H_
#define _YA_HAL_NET_H_

#include "lwip/netdb.h"
#include "lwip/inet.h"
#include "lwipopts.h"
#include "lwip/sockets.h"

uint32_t ya_hal_htonl(uint32_t hostlong);

uint16_t ya_hal_htons(uint16_t n);

uint16_t ya_hal_ntohs(uint16_t n);

uint32_t ya_hal_inet_addr(const char *cp);

int32_t ya_hal_get_ip_by_name(const char *host_name, uint32_t *dest_ip);

int32_t ya_hal_socket_create(int32_t domain, int32_t socket_type, int32_t protocol);

int32_t ya_hal_socket_close(int32_t fd);

int32_t ya_hal_socket_bind(int32_t fd, const struct sockaddr *addr, socklen_t addrlen);

int32_t ya_hal_socket_connect(int32_t fd, const struct sockaddr *addr, socklen_t addrlen);

int32_t ya_hal_socket_connect_block(int32_t fd, const struct sockaddr *addr, socklen_t addrlen);

int32_t ya_hal_socket_listen(int32_t fd, int32_t backlog);

int32_t ya_hal_socket_recv(int32_t fd, void *buf, size_t nbyte, int32_t flags);

int32_t ya_hal_socket_send(int32_t fd, const void *buf, size_t nbyte, int32_t flags);

int32_t ya_hal_socket_sendto(int32_t fd, const void *buf, size_t len, int32_t flags, const struct sockaddr *target_addr, socklen_t target_len);

int32_t ya_hal_socket_recvform(int32_t fd, const void *buf, size_t len, int32_t flags, const struct sockaddr *source_addr, socklen_t *source_len);

int32_t ya_tcp_client_connect(int32_t *sock, uint32_t dest_ip, uint16_t dest_port, uint32_t rece_timeout);

int32_t ya_tcp_client_connect_block(int32_t *sock, uint32_t dest_ip, uint16_t dest_port, uint32_t rece_timeout);

int32_t ya_server_set_up(int32_t *sock, uint16_t server_port, int32_t backlog);

int32_t ya_udp_broadcast_set_up(int32_t *sock, uint16_t port);

int32_t ya_udp_server(uint16_t port, int timeout);


#endif

