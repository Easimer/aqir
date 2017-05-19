/*
 * aqir.cpp - Aqir Entry Point
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#include <aqir.h>
#include <wow.h>
#include <objmgr.h>
#include <player.h>
#include <x11.h>
#include <obj.h>
#include <utils.h>
#include <camera.h>

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <ctime>

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

void throw_bobber(void)
{
	x11_open();
	x11_kbkey(KEY_4);
	x11_close();
	sleep(1);
}

void fuck(void)
{
	uintptr_t EntityList = *reinterpret_cast<uintptr_t*>(BASEADDR + ENTLIST);
	uintptr_t FirstEntity = *reinterpret_cast<uintptr_t*>(EntityList + OBJMGR_0);
	uintptr_t EntityPtr = FirstEntity;
	while((EntityPtr != 0) && ((EntityPtr & 1) == 0) && IsPointerValid(EntityPtr))
	{
		uintptr_t cache;
		char* name;

		uint64_t type = READ_UINT64(EntityPtr + 0x18);
		if(type != 5)
			goto cont;

		cache = *reinterpret_cast<uintptr_t*>(EntityPtr + 824);
		std::cout << "Cache: " << std::hex << cache << std::endl;
		name = *reinterpret_cast<char**>(cache + 208);
		std::cout << "Name: " << std::hex << (uintptr_t)name << std::endl;


		std::cout << "=========" << std::endl;
		std::cout << name << std::endl;

		cont:
		EntityPtr = *reinterpret_cast<uintptr_t*>(EntityPtr + OBJMGR_N);
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

	// wait; otherwise the X11 window would get in an invalid state for some reason
	sleep(10);
	std::cout << "WoW build: " << wow::game::GetGameBuild() << std::endl;
	std::cout << "=============" << std::endl;

	std::cout << "Initializing telnet shell" << std::endl;

	if(pthread_create(&aqir_net_thread, NULL, aqir_net_thread_func, NULL))
	{
		exit(666);
	}

	//CXOrg win("World of Warcraft");

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

		if(!isFishing)
		{
			x11_kbkey(KEY_4);
		}

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
							std::cerr << "[!!!] Bobber was out of view, please correct the camera angles" << std::endl;
						}
						sleep(1);
						
					}
				}
			}
			sleep(1);
		}
		// watch if the player has logged out; avoid SIGSEGV
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
/*
	while(1)
	{
		if(!wow::game::IsBotEnabled())
		{
			sleep(1);
			continue;
		}

		sleep(2); // GCD, etc.		

		if(wait_time == 0xFF)
		{
			std::cout << "bot: train first" << std::endl;
			for(int i = 0; i < 3; i++)
			{
				throw_bobber();
				time_t start = time(NULL);
				while(CWowPlayer::IsFishing());
				time_t end =  time(NULL) - start + 1;
				if(wait_time > end)
				{
					wait_time = end;
				}
				std::cout << end << std::endl;
				sleep(2);
			}

			wait_time = wait_time;
			
			std::cout << "bot: train res: " << wait_time << std::endl;
			continue;
		}

		std::cout << "bot: throw bobber" << std::endl;
		throw_bobber();

		std::cout << "bot: wait..." << std::endl;
		sleep(wait_time + ((rand() % 4) - 2) - 3);

		std::cout << "bot: begin looting" << std::endl;
		x11_open();

		bool looting = false;

		x11_kbhold(KEY_LSHIFT);

		for(unsigned y = 440; y < 640; y+= 2)
		{
			for(unsigned x = 660; x < 1160; x+= 2)
			{
				x11_mclick(x, y);
				looting = CWow::IsPlayerLooting();
				if(looting) break;
			}
			if(looting) break;
		}

		if(looting)
		{
			sleep(1);
			std::cout << "bot: loot" << std::endl;
		}

		x11_kbrel(KEY_LSHIFT);

		x11_close();
		std::cout << "bot: stop looting" << std::endl;

		continue;
	}
*/
	return NULL;
}
