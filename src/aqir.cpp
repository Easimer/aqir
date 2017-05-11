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

static void aqir_init(void) __attribute__((constructor));
static void * aqir_thread_func(void *);

static void aqir_init(void)
{

	if(pthread_create(&aqir_thread, NULL, aqir_thread_func, NULL))
	{
		exit(666);
	}
}

static void * aqir_thread_func(void *param)
{
	sleep(5);
	std::cout << "=============" << std::endl;
	std::cout << ".:: Aqir by easimer ::." << std::endl;

	pid_t p = getpid();
	std::cout << "Linked to process: " << p << std::endl;

	char fnamebuf[256];
	char pnamebuf[256];
	memset(fnamebuf, 0, 256);
	memset(pnamebuf, 0, 256);

	snprintf(fnamebuf, 256, "/proc/%d/cmdline", p);

	FILE* f = fopen(fnamebuf, "r");
	fread(pnamebuf, 128, 1, f);
	fclose(f);

	std::cout << "Process name: " << pnamebuf << std::endl;
	sleep(10);
	std::cout << "WoW build: " << CWow::GetGameBuild() << std::endl;
	std::cout << "=============" << std::endl;

	uintptr_t player;
	while(!(player = CWow::GetPlayerPointer()))
	{
		sleep(1);
	}

	std::cout << "Player offset is " << std::hex << player << std::endl;

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
		sleep(1);
	}

	return NULL;
}
