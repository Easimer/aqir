/*
 * wow.h - CWow header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#pragma once

#include <stdint.h>
#include <obj.h>
#include <string>

namespace wow {

	class game {
	public:
		static uintptr_t GetObjMgrBase(void);
		static uintptr_t GetObjMgr(void);
		static uintptr_t GetObjMgrFirst(void);

		static unsigned short GetGameBuild(void);
		static uintptr_t GetPlayerPointer(void);
		static bool IsPlayerInGame(void);
		static bool IsPlayerLooting(void);

		static void EnableBot(void);
		static void DisableBot(void);
		static bool IsBotEnabled(void);

		static uintptr_t GetEntityList(void);
		static uintptr_t GetFirstEntityAddr(void);
	};

}

#define GAMEBUILD_OFF (0x1274FE4)
#define PLAYERPTR_OFF (0x141C2D0)

#define BASEADDR (0x140000000)
#define CLIENTCONN (0x009BE678)
