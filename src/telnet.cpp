/*
 * telnet.cpp - Interactive debug shell over TCP/IP
 * Author: Daniel Meszaros <easimer@gmail.com>
 * This file is part of Aqir, see LICENSE
 */

#include <aqir.h>
#include <wow.h>
#include <player.h>
#include <camera.h>
#include <x11.h>
#include <objmgr.h>
#include <telnet.h>
#include <utils.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>
#include <sstream>
#include <vector>
#include <iterator>

void telnet_process_client(int fd);

int aqir_net_ls = 0, aqir_net_cs = 0;

void aqir_net_fini(void)
{
	if(aqir_net_ls)
		close(aqir_net_ls);
	if(aqir_net_cs)
		close(aqir_net_cs);
}

void * aqir_net_thread_func(void *param)
{
	int sockfd;
	struct sockaddr_in self;

	sleep(2);

	// open socket

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "network fail: socket()" << std::endl;
		pthread_exit(NULL);
		return NULL;
	}

	// make socket reusable
	int en = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(int)) < 0)
	{
		std::cout << "network fail: SO_REUSEADDR" << std::endl;
		pthread_exit(NULL);
		return NULL;
	}

	// init structs, bind to 0.0.0.0:1666

	memset(&self, 0, sizeof(self));
	self.sin_family = AF_INET;
	self.sin_port = htons(1666);
	self.sin_addr.s_addr = INADDR_ANY;

	if(bind(sockfd, reinterpret_cast<struct sockaddr*>(&self), sizeof(self)) != 0)
	{
		std::cout << "network fail: bind()" << std::endl;
		perror("cause");
		pthread_exit(NULL);
		return NULL;
	}

	// listen on socket

	if(listen(sockfd, 20) != 0)
	{
		std::cout << "network fail: listen()" << std::endl;
		pthread_exit(NULL);
		return NULL;
	}

	aqir_net_ls = sockfd;

	std::cout << "network: accepting connections" << std::endl;

	while(true)
	{
		int clientfd;

		struct sockaddr_in client_addr;
		socklen_t addrlen = sizeof(client_addr);

		// accept connection
		clientfd = accept(sockfd, reinterpret_cast<struct sockaddr*>(&client_addr), &addrlen);
		std::cout << "network: client " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << " connected"  << std::endl;
		aqir_net_cs = clientfd;
		telnet_process_client(clientfd);
		close(clientfd);
		aqir_net_cs = 0;
	}

	aqir_net_ls = 0;

}

std::vector<std::string> split(std::string& s)
{
	std::string buf;
	std::stringstream ss(s);

	std::vector<std::string> vec;

	while(ss >> buf)
	{
		if(!buf.empty())
			vec.push_back(buf);
	}
	return vec;
}

void telnet_err(int fd, int errcode)
{
	const char* err_nocmd = "no such command\n";
	const char* err_insuff = "not enough arguments\n";
	const char* err_generic = "error\n";
	const char* err_ok = "OK\n";

	switch(errcode)
	{
		case TELNET_ERR_INSUF:
			send(fd, err_insuff, strlen(err_insuff), 0);
			break;
		case TELNET_ERR_NOSUCH:
			send(fd, err_nocmd, strlen(err_nocmd), 0);
			break;
		case TELNET_ERR_OK:
			send(fd, err_ok, strlen(err_ok), 0);
			break;
		default:
			send(fd, err_generic, strlen(err_generic), 0);
			break;
	}
}

// Command handlers

TELNET_DEF_CMD_FUNCTION(enable)
{
	wow::game::EnableBot();
	std::cout << "network: bot enabled remotely" << std::endl;
	return true;
}

TELNET_DEF_CMD_FUNCTION(disable)
{
	wow::game::DisableBot();
	std::cout << "network: bot disabled remotely" << std::endl;
	return true;
}

TELNET_DEF_CMD_FUNCTION(dmem)
{
	std::stringstream buf;
	uintptr_t addr;
	size_t size;
	bool sent = false;

	if(tok.size() < 3)
	{
		telnet_err(fd, TELNET_ERR_INSUF);
		return false;
	}

	addr = std::stoull(tok[1], nullptr, 16);
	size = std::stoull(tok[2], nullptr, 10);

	if(addr == 0 || !IsPointerValid(addr))
	{
		return false;
	}

	std::cout << "network: memory dump from " <<
		std::hex << addr << " to " <<
		std::hex << (addr + size) <<
		std::dec << std::endl;

	buf << std::hex;

	for(size_t i = 0; i < size; i++)
	{
		uint8_t byte = reinterpret_cast<uint8_t*>(addr)[i];
		buf << std::hex << (unsigned)byte << ' ';
		if((i & 15) == 0)
			sent = false;
		if((i & 15) == 15)
		{
			buf << std::endl;
			std::string s = buf.str();
			send(fd, s.c_str(), s.size(), 0);
			buf.clear();
			sent = true;
		}
	}
	if(!sent)
	{
		std::string s = buf.str();
		send(fd, s.c_str(), s.size(), 0);
	}
	return true;
}

TELNET_DEF_CMD_FUNCTION(xkbe)
{
	std::cout << "network: X11 keypress test" << std::endl;
	sleep(2);
	x11_open();
	x11_kbkey(KEY_4);
	x11_close();
	return true;
}

TELNET_DEF_CMD_FUNCTION(jump)
{
	if(!wow::localplayer::IsInGame())
		return false;
	std::cout << "network: ascension" << std::endl;
	wow::localplayer::SetZ(wow::localplayer::GetZ() + 10.f);
	return true;
}

TELNET_DEF_CMD_FUNCTION(descend)
{
	if(!wow::localplayer::IsInGame())
		return false;
	std::cout << "network: descension" << std::endl;
	wow::localplayer::SetZ(wow::localplayer::GetZ() - 10.f);
	return true;
}

TELNET_DEF_CMD_FUNCTION(pos)
{
	float ppos[3];
	float cpos[3];
	std::stringstream buf;

	if(!wow::localplayer::IsInGame())
		return false;

	std::cout << "network: local player position" << std::endl;

	ppos[0] = wow::localplayer::GetX();
	ppos[1] = wow::localplayer::GetY();
	ppos[2] = wow::localplayer::GetZ();
	
	wow::camera::GetPosition(cpos);

	buf << "Player: " << ppos[0] << ' ' << ppos[1] << ' ' << ppos[2] << std::endl;
	buf << "Camera: " << cpos[0] << ' ' << cpos[1] << ' ' << cpos[2] << std::endl;

	std::string s = buf.str();

	std::cout << s;
	send(fd, s.c_str(), s.size(), 0);
	return true;
}

TELNET_DEF_CMD_FUNCTION(players)
{
	if(!wow::localplayer::IsInGame())
		return false;
	wow::objmgr om;

	std::cout << "network: players listed" << std::endl;

	std::vector<uintptr_t> players = om.GetPlayers();
	for(auto& pplayer : players)
	{
		wow::player p(pplayer);
		std::string buf;
		std::stringstream res;
		res << std::hex << pplayer << ' ';
		res << p.name() << "\n(" << p.getx() << ',' << p.gety() << ',' << p.getz() << ')' << std::endl;
		buf = res.str();
		send(fd, buf.c_str(), buf.size(), 0);
	}
	return true;
}

TELNET_DEF_CMD_FUNCTION(whoami)
{
	if(!wow::localplayer::IsInGame())
		return false;
	std::cout << "network: whoami" << std::endl;
	auto name = wow::localplayer::GetName();
	name.append("\n");
	send(fd, name.c_str(), name.size(), 0);
	return true;
}

TELNET_DEF_CMD_FUNCTION_DEC(help)

TELNET_DEF_CMD_START
// Bot controls
TELNET_DEF_CMD(enable, "Enable the fishing bot")
TELNET_DEF_CMD(disable, "Disable the fishing bot")
// Information
TELNET_DEF_CMD(pos, "Print local player's and the camera's position")
TELNET_DEF_CMD(players, "Print players and their position")
TELNET_DEF_CMD(whoami, "Print local player's name")
TELNET_DEF_CMD(help, "Print this text")
// Cheats
TELNET_DEF_CMD(jump, "CHEAT: Increment Z position (height) by 10 units")
TELNET_DEF_CMD(descend, "CHEAT: Decrement Z position (height) by 10 units")
// Debug
TELNET_DEF_CMD(dmem, "DEBUG: Dump memory: <address> <size>")
//TELNET_DEF_CMD(xmbe, "X11 Mouse Button Event test")
TELNET_DEF_CMD(xkbe, "DEBUG: X11 Keyboard Button Event test")
TELNET_DEF_CMD_END

;

TELNET_DEF_CMD_FUNCTION(help)
{
	std::stringstream buf;
	buf << "aqir shell" << std::endl;

	auto jte = TELNET_DEF_CMD_FIRST;

	while(TELNET_DEF_CMD_HASH(jte) != 0 && TELNET_DEF_CMD_CALLBACK(jte) != NULL)
	{
		buf << TELNET_DEF_CMD_NAME(jte) << " - " << TELNET_DEF_CMD_HELP(jte) << std::endl;
		jte++;
	}

	std::string s = buf.str();

	send(fd, s.c_str(), s.size(), 0);
	return true;
}

void telnet_process_command(int fd, std::string& cmd)
{
		auto tok = split(cmd);
		size_t tok0hash = std::hash<std::string>{}(tok[0]);
		auto jte = TELNET_DEF_CMD_FIRST;

		while(TELNET_DEF_CMD_HASH(jte) != 0 && TELNET_DEF_CMD_CALLBACK(jte) != NULL)
		{
			if(TELNET_DEF_CMD_HASH(jte) == tok0hash)
			{
				if(TELNET_DEF_CMD_CALL(jte, fd, tok))
					telnet_err(fd, TELNET_ERR_OK);
				return;
			}
			jte++;
		}

		telnet_err(fd, TELNET_ERR_NOSUCH);

		return;
}

void telnet_process_client(int fd)
{
	int rc;
	std::stringstream ss;
	const char* prompt = "# ";

	std::vector<std::string> help_hack = {"help"};

	telnet_cmd_help(fd, help_hack);

	send(fd, prompt, 2, 0);

	while(true) {
		char buf[32];
		memset(buf, 0, 32);
		rc = recv(fd, buf, 32, 0);
		if(rc == 0)
			return;
		ss << buf;
		std::string str = ss.str();
		if(str.find_first_of('\n') != std::string::npos)
		{
			if(str.size() > 1)
				telnet_process_command(fd, str);
			ss.str("");
			ss.clear();
			send(fd, prompt, 2, 0);
		}
	}
}
