/*
 * net.h - Telnet header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * This file is part of Aqir, see LICENSE
 */
#pragma once

void * aqirl_net_thread_func(void *); 

#define TELNET_ERR_INSUF 0
#define TELNET_ERR_NOSUCH 1
#define TELNET_ERR_OK 0xFF
