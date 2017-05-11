/*
 * telnet.cpp - Interactive debug shell over TCP/IP
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */

#include <aqir.h>
#include <wow.h>
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

	sleep(10);

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "network fail: socket()" << std::endl;
		pthread_exit(NULL);
		return NULL;
	}

	int en = 1;

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(int)) < 0)
	{
		std::cout << "network fail: SO_REUSEADDR" << std::endl;
		pthread_exit(NULL);
		return NULL;
	}

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
}