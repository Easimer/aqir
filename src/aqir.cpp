/*
 * aqir.cpp - Aqir Entry Point
 * Author: Daniel Meszaros <easimer@gmail.com>
 * This file is part of Aqir, see LICENSE
 */
#include <aqir.h>
#include <wow.h>
#include <objmgr.h>
#include <player.h>
#include <x11.h>
#include <obj.h>
#include <utils.h>
#include <camera.h>
#include <telnet.h>

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <ctime>

#ifndef AQIR_LOADED
static void aqir_init(void) __attribute__((constructor));
#warning "Aqir Loader support disabled"
#endif

static void aqir_fini(void) __attribute__((destructor));

extern "C" {
	pthread_t aqir_thread;
	pthread_t aqir_net_thread;

	const int aqir_version_ = 1;
	const int* aqir_version = &aqir_version_;
	const char* aqir_version_str = "Aqir 0.1.3 by easimer";
	void * aqir_thread_func(void *);
}

extern void * aqir_net_thread_func(void *); 

// DLL constructor (called only when not loaded using aqirl)
static void aqir_init(void)
{
	if(pthread_create(&aqir_thread, NULL, aqir_thread_func, NULL))
	{
		exit(666);
	}
}

// DLL destructor
static void aqir_fini(void)
{
	pthread_cancel(aqir_net_thread);
	pthread_join(aqir_net_thread, NULL);
	aqir_net_fini();
	x11_close();
}

void * aqir_thread_func(void *param)
{
	#ifndef AQIR_LOADED
	// Wait; printing causes SIGSEGV for some reason
	sleep(5);
	#endif
	std::cout << "=============" << std::endl;
	std::cout << ".:: Aqir by easimer ::." << std::endl;

	#ifndef AQIR_LOADED

	// Read PID

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
	// Wine loads aqir.so for a fuckton of other processes and we need to open the
	// telnet port in the game only

	if(strcmp(exename, "Wow-64.exe") != 0)
	{
		std::cout << "This is not WoW!!!" << std::endl;
		pthread_exit(NULL);
	}

	//

	std::cout << "Process name: " << exename << std::endl;

	#endif /* AQIR_LOADED */

	// wait; otherwise the X11 window would get in an invalid state for some reason
	sleep(10);
	std::cout << "WoW build: " << wow::game::GetGameBuild() << std::endl;
	std::cout << "=============" << std::endl;

	std::cout << "Initializing telnet shell" << std::endl;

	if(pthread_create(&aqir_net_thread, NULL, aqir_net_thread_func, NULL))
	{
		exit(666);
	}

	// acquire player base address

	uintptr_t player;
	uintptr_t player2;
	while(!(player = wow::game::GetPlayerPointer()))
	{
		sleep(1);
	}

	std::cout << "Player offset is " << std::hex << player << std::endl;

	srand(time(NULL)); // seed RNG

	while(!wow::game::IsBotEnabled())
		sleep(1);

	// initialize these variables

	bool wasMounted = wow::localplayer::IsMounted();
	bool wasFishing = wow::localplayer::IsFishing();
	bool wasLooting = wow::localplayer::IsLooting();

	wow::objmgr om;
	wow::gameobj bobber;
	bool hasbobber = false;
	bool fired = false;
	int mx = -1; int my = -1;

	while(true)
	{
		while(!wow::game::IsBotEnabled())
			sleep(1);

		bool isMounted = wow::localplayer::IsMounted();
		bool isFishing = wow::localplayer::IsFishing();
		bool isLooting = wow::localplayer::IsLooting();

		/*if(!isFishing)
		{
			x11_kbkey(KEY_4);
		}*/

		if(isMounted != wasMounted)
		{
			std::cout << "Player has " << (wasMounted ? "unmounted" : "mounted") << std::endl;
			wasMounted = isMounted;
		}
		if(isFishing != wasFishing)
		{
			std::cout << "Player has " << (wasFishing ? "exited" : "entered") << " fishing state" << std::endl;
			if(!wasFishing)
			{
				sleep(1);
				// Find my fishing bobber
				const char* fbn = "Fishing Bobber";
				uintptr_t bobberaddr = om.GetOwnedObjectByName(fbn, O_ANY);
				if(bobberaddr)
				{
					std::cout << "Bobber found" << std::endl;
					bobber = wow::gameobj(bobberaddr);
					hasbobber = true;
				}
				else
				{
					std::cout << "Bobber not found" << std::endl;
				}
			}
			if(wasFishing)
			{
				hasbobber = false;
			}
			wasFishing = isFishing;
		}
		if(isLooting != wasLooting)
		{
			std::cout << "Player has " << (wasLooting ? "exited" : "entered") << " looting state" << std::endl;
			wasLooting = isLooting;
		}
		if(!isFishing)
			sleep(1);
		else
		{
			if(hasbobber)
			{
				if(bobber.isbobbing())
				{
					if(!fired)
					{
						float bpos[3]; float spos[2];
						// Project the Fishing Bobber's position on the screen
						fired = true;

						bpos[0] = bobber.getx();
						bpos[1] = bobber.gety();
						bpos[2] = bobber.getz();

						wow::camera::WorldToScreen(bpos, spos);

						mx = spos[0];
						my = spos[1];

						if(mx > -1 && my > -1)
						{
							x11_open();
							x11_kbhold(KEY_LSHIFT);
							x11_mclick(mx, my);
							x11_kbrel(KEY_LSHIFT);
							x11_close();
						}
						else
						{
							std::cerr << "[!!!] Bobber was out of view, please move the camera" << std::endl;
						}
						sleep(1);
						
					}
				}
			}
			sleep(1);
		}
		// Player pointer changes on map reloads
		// Watch out for this to avoid a SIGSEGV
		player2 = wow::game::GetPlayerPointer();
		if(!player2)
		{
			std::cout << "Player has logged out" << std::endl;
			while(!(player = wow::game::GetPlayerPointer()))
				sleep(1);
			std::cout << "Player has logged back" << std::endl;
		}
		if(fired)
		{
			sleep(1);
			fired = false;
		}
	}
	return NULL;
}
