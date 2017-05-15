/*
 * aqir.cpp - Aqir Entry Point
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#include <aqir.h>
#include <wow.h>
#include <x11.h>
 #include <obj.h>

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <time.h>

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
	std::cout << "Traversing the motherfucking entitylist" << std::endl;
	uintptr_t obj;
	obj = *(uintptr_t*)(0x141387048 + 0x18);
	while(obj /*&& ((obj & 1) == 0)*/)
	{
		std::cout << "OBJ FOUND" << std::endl;
		uintptr_t doff = *(uintptr_t*)(obj + 0x8);
		uint8_t type = *(uint8_t*)(doff + 24);
		std::cout << "Type: " << (unsigned)type << std::endl;
	}
	std::cout << "END" << std::endl;
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

	//CXOrg win("World of Warcraft");

	// acquire player base address

	uintptr_t player;
	uintptr_t player2;
	while(!(player = CWow::GetPlayerPointer()))
	{
		sleep(1);
	}

	std::cout << "Player offset is " << std::hex << player << std::endl;

	srand(time(NULL)); // seed RNG

	// initialize these variables

	bool wasMounted = CWowPlayer::IsMounted();
	bool wasFishing = CWowPlayer::IsFishing();
	bool wasLooting = CWow::IsPlayerLooting();

	unsigned wait_time = 0xFF;


	// TEST

	/*sleep(5);

	uintptr_t objcur = CWow::GetObjMgrFirst();
	if(objcur == NULL)
	{
		std::cout << "AAAAAAAAAAAH" << std::endl;
	}
	CWowObject obj(objcur);
	std::cout << std::dec << obj.GetGUID() << std::endl;*/

	// TEST

	while(!CWow::IsBotEnabled())
		sleep(1);

	std::cout << std::dec << CWowPlayer::GetX() << std::endl;
	std::cout << std::dec << CWowPlayer::GetY() << std::endl;

	//std::cout << CWowPlayer::GetName() << std::endl;
	std::cout << "Level " << std::dec << CWowPlayer::GetLevel() << std::endl;

	fuck();

	//std::cout << "First entity" << std::endl;
	//auto ent = reinterpret_cast<struct wow_ent*>(CWow::GetFirstEntityAddr());
	//std::cout << "Entity type: " << std::hex << ent->type << std::endl;

	while(true)
	{
		bool isMounted = CWowPlayer::IsMounted();
		bool isFishing = CWowPlayer::IsFishing();
		bool isLooting = CWow::IsPlayerLooting();

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
		if(isLooting != wasLooting)
		{
			std::cout << "Player has " << (wasLooting ? "exited" : "entered") << " looting state" << std::endl;
			wasLooting = isLooting;
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
		{
			std::cout << "Player has logged out" << std::endl;
			while(!(player = CWow::GetPlayerPointer()))
				sleep(1);
			std::cout << "Player has logged back" << std::endl;
		}
	}

	return NULL;

	while(1)
	{
		if(!CWow::IsBotEnabled())
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

	return NULL;
}
