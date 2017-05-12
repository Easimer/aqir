/*
 * telnet.cpp - Interactive debug shell over TCP/IP
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */

#include <aqir.h>
#include <wow.h>
#include <x11.h>
#include <telnet.h>
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

void * aqir_net_thread_func(void *param)
{
	int sockfd;
	struct sockaddr_in self;

	sleep(5);

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

	if(bind(sockfd, (struct sockaddr*)&self, sizeof(self)) != 0)
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

	std::cout << "network: accepting connections" << std::endl;

	while(true)
	{
		int clientfd;

		struct sockaddr_in client_addr;
		socklen_t addrlen = sizeof(client_addr);

		// accept connection
		clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
		std::cout << "network: client " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << " connected"  << std::endl;
		telnet_process_client(clientfd);
		close(clientfd);
	}

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

void telnet_process_client(int fd)
{
	int rc;
	char buf[256];
	std::string motd = "aqsh\navailable commands: enable, disable, testkey, dump\n\n# ";
	std::string err_nocmd = "no such command\n";
	std::string err_insuff = "not enough arguments\n";
	send(fd, motd.c_str(), motd.size(), 0);

	memset(buf, 0, 256);

	rc = recv(fd, buf, 256, 0);

	if(rc == 0)
		return;

	std::string cmd(buf);
	auto tok = split(cmd);
	for(auto& t : tok)
	{
		std::cout << t << std::endl;
	}
	if(tok[0] == "enable")
	{
		CWow::EnableBot();
		std::cout << "network: bot enabled remotely" << std::endl;
	}
	else if(tok[0] == "disable")
	{
		CWow::DisableBot();
		std::cout << "network: bot disabled remotely" << std::endl;
	}
	else if(tok[0] == "testkey")
	{
		sleep(2);
		x11_open();
		x11_kbkey(KEY_4);
		x11_close();
		std::cout << "network: key test issued" << std::endl;
	}
	else if(tok[0] == "dump")
	{
		if(tok.size() < 3)
		{
			send(fd, err_insuff.c_str(), err_insuff.size(), 0);
			return;
		}

		uintptr_t addr;
		size_t size;

		addr = std::stoull(tok[1], nullptr, 16);
		size = std::stoull(tok[2], nullptr, 10);

		std::stringstream buf;

		buf << std::hex;

		for(size_t i = 0; i < size; i++)
		{
			uint8_t byte = ((uint8_t*)addr)[i];
			buf << std::to_string(byte) << ' ';
			if((i & 15) == 15)
			{
				buf << std::endl;
				std::string s = buf.str();
				send(fd, s.c_str(), s.size(), 0);
				buf.clear();
			}
		}

	}
	else
	{
		send(fd, err_nocmd.c_str(), err_nocmd.size(), 0);
	}
}