/*
 * wow.h - CWow header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * This file is part of Aqir, see LICENSE
 */
#pragma once

#include <stdint.h>
#include <obj.h>
#include <string>
#include <offsets.h>

namespace wow {

	class game {
	public:
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
