/*
 * telnet.h - Telnet header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * This file is part of Aqir, see LICENSE
 */
#pragma once

//void * aqir_net_thread_func(void *); 
void aqir_net_fini(void);

#define TELNET_ERR_INSUF 0
#define TELNET_ERR_NOSUCH 1
#define TELNET_ERR_OK 0xFF

typedef bool(*telnet_chf)(int, std::vector<std::string>&);

typedef struct {
	std::string str;
	std::size_t str_hash;
	telnet_chf callback;
	std::string help;
} telnet_jte;

//
#define JTE_HASHFN (std::hash<std::string>{})
//
#define TELNET_DEF_CMD_START	telnet_jte telnet_cmd_handlers[] = {
//
#define TELNET_DEF_CMD_END		{"", 0, NULL, ""}};
//
#define TELNET_DEF_CMD(name, help) {#name, JTE_HASHFN(#name), telnet_cmd_ ## name, help},

#define TELNET_DEF_CMD_FIRST (telnet_cmd_handlers)
#define TELNET_DEF_CMD_NAME(e) (e->str)
#define TELNET_DEF_CMD_HASH(e) (e->str_hash)
#define TELNET_DEF_CMD_CALLBACK(e) (e->callback)
#define TELNET_DEF_CMD_CALL(e, fd, tokens) (e->callback(fd, tokens))
#define TELNET_DEF_CMD_FUNCTION(name) bool telnet_cmd_ ## name (int fd, std::vector<std::string>& tok)
#define TELNET_DEF_CMD_FUNCTION_DEC(name) TELNET_DEF_CMD_FUNCTION(name);
#define TELNET_DEF_CMD_HELP(e) (e->help)
