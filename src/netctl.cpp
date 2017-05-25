/*
 * netctl.cpp - Aqir Control over UDP
 * Author: Daniel Meszaros <easimer@gmail.com>
 * This file is part of Aqir, see LICENSE
 */

#include <netctl.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int aqir_netctl_ls = 0;

void * aqir_netctl_thread_func(void *param)
{
	int sockfd;
	struct sockaddr_in self;

	sleep(2);

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		std::cout << "netctl fail: socket()" << std::endl;
		pthread_exit(NULL);
		return NULL;
	}

	// make socket reusable
	int en = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(int)) < 0)
	{
		std::cout << "netctl fail: SO_REUSEADDR" << std::endl;
		pthread_exit(NULL);
		return NULL;
	}

	memset(&self, 0, sizeof(self));
	self.sin_family = AF_INET;
	self.sin_port = htons(1668);
	self.sin_addr.s_addr = INADDR_ANY;

	if(bind(sockfd, reinterpret_cast<struct sockaddr*>(&self), sizeof(self)) != 0)
	{
		std::cout << "netctl fail: bind()" << std::endl;
		perror("cause");
		pthread_exit(NULL);
		return NULL;
	}

	aqir_netctl_ls = sockfd;

	std::cout << "netctl: accepting connections" << std::endl;

	struct sockaddr_in caddr;
	socklen_t clen;
	while(true)
	{
		uint8_t hdrbuf[16];
		uint8_t pktbuf[64];
		int n;
		netctl_pkthdr_t* hdr = (netctl_pkthdr_t*)hdrbuf;

		n = recvfrom(sockfd, hdrbuf, 16, 0, (struct sockaddr*)&caddr, &clen);
		if(n < 16)
		{
			perror("netctl recv fault");
			continue;
		}


	}
}
