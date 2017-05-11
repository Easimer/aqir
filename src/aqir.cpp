/*
 * aqir.cpp - Aqir Entry Point
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#include <aqir.h>
#include <wow.h>
#include <x11.h>

#include <iostream>
#include <unistd.h>
#include <cstring>

pthread_t aqir_thread;
pthread_t aqir_net_thread;

static void aqir_init(void) __attribute__((constructor));
static void * aqir_thread_func(void *);

extern void * aqir_net_thread_func(void *); 

static void aqir_init(void)
{
	if(pthread_create(&aqir_thread, NULL, aqir_thread_func, NULL))
	{
		exit(666);
	}
}

static void * aqir_thread_func(void *param)
{
	// Wait; printing causes SIGSEGV for some reason
	sleep(5);
	std::cout << "=============" << std::endl;
	std::cout << ".:: Aqir by easimer ::." << std::endl;

	// Read PID

	pid_t p = getpid();
	std::cout << "Linked to process: " << p << std::endl;

	// Read process name

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
	// Wine loads aqir.so for a fuckton of other processes and we need to open the
	// telnet port in the game only

	if(strcmp(exename, "Wow-64.exe") != 0)
	{
		std::cout << "This is not WoW!!!" << std::endl;
		pthread_exit(NULL);
	}

	std::cout << "Process name: " << exename << std::endl;

	// wait; otherwise the X11 window would get in an invalid state for some reason
	sleep(10);
	std::cout << "WoW build: " << CWow::GetGameBuild() << std::endl;
	std::cout << "=============" << std::endl;

	std::cout << "Initializing telnet shell" << std::endl;

	if(pthread_create(&aqir_net_thread, NULL, aqir_net_thread_func, NULL))
	{
		exit(666);
	}

	// acquire player base address

	uintptr_t player;
	uintptr_t player2;
	while(!(player = CWow::GetPlayerPointer()))
	{
		sleep(1);
	}

	std::cout << "Player offset is " << std::hex << player << std::endl;

	// initialize these variables

	bool wasMounted = CWowPlayer::IsMounted();
	bool wasFishing = CWowPlayer::IsFishing();

	while(1)
	{
		bool isMounted = CWowPlayer::IsMounted();
		bool isFishing = CWowPlayer::IsFishing();

		if(isMounted != wasMounted)
		{
			std::cout << "Player has " << (wasMounted ? "unmounted" : "mounted") << std::endl;
			wasMounted = isMounted;
		}
		if(isFishing != wasFishing)
		{
			std::cout << "Player has " << (wasFishing ? "exited" : "entered") << " fishing state" << std::endl;
			wasFishing = isFishing;
		}
		if(!isFishing)
			sleep(1);
		else
		{
			sleep(1);
		}
		// watch if the player has logged out; avoid SIGSEGV
		player2 = CWow::GetPlayerPointer();
		if(!player2)
			while(!(player = CWow::GetPlayerPointer()))
				sleep(1);
	}

	return NULL;
}
