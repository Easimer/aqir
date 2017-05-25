/*
 * net.cpp - Telnet shell
 * Author: Daniel Meszaros <easimer@gmail.com>
 * This file is part of Aqir, see LICENSE
 */
#include <aqirl.h>
#include <net.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>
#include <sstream>
#include <vector>
#include <iterator>

void aqirl_net_process_client(int fd);

void * aqirl_net_thread_func(void *)
{
	int sockfd;
	struct sockaddr_in self;

	sleep(2);
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "aqirl: network fail: socket()" << std::endl;
		pthread_exit(NULL);
		return NULL;
	}

	// make socket reusable
	int en = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(int)) < 0)
	{
		std::cout << "aqirl: network fail: SO_REUSEADDR" << std::endl;
		pthread_exit(NULL);
		return NULL;
	}

	memset(&self, 0, sizeof(self));
	self.sin_family = AF_INET;
	self.sin_port = htons(1667);
	self.sin_addr.s_addr = INADDR_ANY;

	if(bind(sockfd, reinterpret_cast<struct sockaddr*>(&self), sizeof(self)) != 0)
	{
		std::cout << "aqirl: network fail: bind()" << std::endl;
		perror("cause");
		pthread_exit(NULL);
		return NULL;
	}

	if(listen(sockfd, 20) != 0)
	{
		std::cout << "aqirl: network fail: listen()" << std::endl;
		pthread_exit(NULL);
		return NULL;
	}

	std::cout << "aqirl: network: accepting connections" << std::endl;

	while(true)
	{
		int clientfd;

		struct sockaddr_in client_addr;
		socklen_t addrlen = sizeof(client_addr);

		// accept connection
		clientfd = accept(sockfd, reinterpret_cast<struct sockaddr*>(&client_addr), &addrlen);
		std::cout << "network: client " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << " connected"  << std::endl;
		aqirl_net_process_client(clientfd);
		close(clientfd);
	}

	return NULL;
}

void aqirl_net_err(int fd, int errcode)
{
	const char* err_nocmd = "no such command\n";
	const char* err_insuff = "not enough arguments\n";
	const char* err_generic = "error\n";
	const char* err_ok = "OK\n";

	switch(errcode)
	{
		case TELNET_ERR_OK:
			send(fd, err_ok, 3, 0);
			break;
		case TELNET_ERR_INSUF:
			send(fd, err_insuff, 21, 0);
			break;
		case TELNET_ERR_NOSUCH:
			send(fd, err_nocmd, 16, 0);
			break;
		default:
			send(fd, err_generic, 6, 0);
			break;
	}
}

static std::vector<std::string> split(std::string& s)
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

void aqirl_net_process_command(int fd, std::string& cmd)
{
	auto tok = split(cmd);
	if(tok[0] == "status")
	{
		const char* status_loaded = "loaded\n";
		const char* status_pending = "pending\n";
		const char* status_error = "error\n";

		if(is_pending())
		{
			send(fd, status_pending, 8, 0);
		}
		else if(is_loaded())
		{
			send(fd, status_loaded, 7, 0);
		}
		else
		{
			send(fd, status_error, 7, 0);
		}
	}
	else if(tok[0] == "reload")
	{
		trigger_reload();
	}
	else
	{
		aqirl_net_err(fd, TELNET_ERR_NOSUCH);
		return;
	}
	aqirl_net_err(fd, TELNET_ERR_OK);
}

void aqirl_net_process_client(int fd)
{
	int rc;
	std::stringstream ss;
	const char* prompt = "# ";
	std::string motd = "aqlsh\navailable commands: reload, status\n\n";
	send(fd, motd.c_str(), motd.size(), 0);

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
			if(str.size() < 1)
				break;

			aqirl_net_process_command(fd, str);
			ss.str("");
			ss.clear();
			send(fd, prompt, 2, 0);
		}
	}
}
