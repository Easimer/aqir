/*
 * aqirl.cpp - Aqir Loader
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#include <iostream>
#include <fstream>

#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>
#include <atomic>

#include <cstring>
#include <cstdlib>

pthread_t aqirl_thread;
pthread_t aqirl_net_thread;
pthread_t aqir_thread;
pthread_t* aqir_net_thread;

std::atomic<bool> ReloadPending(true);
std::atomic<bool> Loaded(false);

static void * aqirl_thread_func(void*);
extern void * aqirl_net_thread_func(void *); 

static void aqirl_init(void) __attribute__((constructor));
static void aqirl_init(void)
{
	if(pthread_create(&aqirl_thread, NULL, aqirl_thread_func, NULL))
	{
		exit(666);
	}
}

void trigger_reload(void)
{
	ReloadPending = true;
}

bool is_pending(void)
{
	return ReloadPending;
}

bool is_loaded(void)
{
	return Loaded;
}

void copyso(void)
{
	std::ifstream src("aqir.so", std::ios::binary);
	std::ofstream dst("aqir.copy.so", std::ios::binary | std::ios::out);

	dst << src.rdbuf();
	src.close();
	dst.close();
}

void * aqirl_thread_func(void* param)
{
	sleep(5);

	std::cout << "=========" << std::endl;
	std::cout << ".:: Aqir Loader by easimer ::." << std::endl;

	pid_t p = getpid();
	std::cout << "Linked to process: " << p << std::endl;

	// Read process name

	// TODO: C++-ify this

	char fnamebuf[256];
	char pnamebuf[256];
	memset(fnamebuf, 0, 256);
	memset(pnamebuf, 0, 256);

	snprintf(fnamebuf, 256, "/proc/%d/cmdline", p);

	FILE* f = fopen(fnamebuf, "r");
	fread(pnamebuf, 128, 1, f);
	fclose(f);

	char* exename = strrchr(pnamebuf, '\\') + 1;

	// check if this is really WoW
	// Wine loads aqirl.so for a fuckton of other processes and we need to open the
	// telnet port in the game only

	if(strcmp(exename, "Wow-64.exe") != 0)
	{
		std::cout << "This is not WoW!!!" << std::endl;
		pthread_exit(NULL);
	}

	std::cout << "Process name: " << exename << std::endl;

	std::cout << "aqirl: Initializing telnet shell" << std::endl;
	if(pthread_create(&aqirl_net_thread, NULL, aqirl_net_thread_func, NULL))
	{
		std::cerr << "aqirl: fail" << std::endl;
		exit(666);
	}

	void* aqir_so = NULL;
	void* (*aqir_main)(void*);
	int ret;

	while(true)
	{
		if(!ReloadPending)
		{
			sleep(1);
			continue;
		}
		std::cout << "== Reload request ==" << std::endl;
		ReloadPending = false;
		if(aqir_so)
		{
			Loaded = false;
			//std::cout << "Canceling aqir threads" << std::endl;
			//pthread_cancel(*aqir_net_thread);
			pthread_cancel(aqir_thread);
			pthread_join(aqir_thread, NULL);
			dlclose(aqir_so);
			std::cout << "Closed aqir.so" << std::endl;
		}
		std::cout << "Opening aqir.so" << std::endl;
		copyso();
		aqir_so = dlopen("aqir.copy.so", RTLD_NOW);
		if(!aqir_so)
		{
			char* err = dlerror();
			if(err)
				std::cerr << "Linking failure: " << err << std::endl;
			else
				std::cerr << "Unknown linking failure" << std::endl;
			continue;
		}
		std::cerr << "aqir.so loaded" << std::endl;

		int aqir_ver = **(int**)dlsym(aqir_so, "aqir_version");
		if(!aqir_ver)
		{
			char* err = dlerror();
			if(err)
				std::cerr << "Linking failure: " << err << std::endl;
			else
				std::cerr << "Unknown linking failure" << std::endl;
			continue;
		}

		char* aqir_ver_str = *(char**)dlsym(aqir_so, "aqir_version_str");
		if(!aqir_ver_str)
		{
			char* err = dlerror();
			if(err)
				std::cerr << "Linking failure: " << err << std::endl;
			else
				std::cerr << "Unknown linking failure" << std::endl;
			continue;
		}
		std::cerr << "Detected version: " << aqir_ver_str << std::endl;

		std::cerr << "Linking aqir_main" << std::endl;

		aqir_main = (void*(*)(void*))dlsym(aqir_so, "aqir_thread_func");
		if(!aqir_main)
		{
			char* err = dlerror();
			if(err)
				std::cerr << "Linking failure: " << err << std::endl;
			else
				std::cerr << "Unknown linking failure" << std::endl;
			continue;
		}
		std::cerr << "Linking net_thread" << std::endl;
		aqir_net_thread = (pthread_t*)dlsym(aqir_so, "aqir_net_thread");
		if(!aqir_net_thread)
		{
			char* err = dlerror();
			if(err)
				std::cerr << "Linking failure: " << err << std::endl;
			else
				std::cerr << "Unknown linking failure" << std::endl;
			continue;
		}
		std::cout << "Starting aqir" << std::endl;
		if((ret = pthread_create(&aqir_thread, NULL, aqir_main, NULL)))
		{
			std::cerr << "Failed to create thread: ";
			switch(ret)
			{
				case EAGAIN:
					std::cerr << "Insufficient resources to create another thread or a system-imposed limit on the number of threads was encountered";
					break;
				case EINVAL:
					std::cerr << "Invalid settings";
					break;
				case EPERM:
					std::cerr << "Permission denied";
					break;
				default:
					std::cerr << "Unknown";
					break;
			}
			std::cerr << std::endl;
			continue;
		}
		Loaded = true;
	}

	return NULL;
}
