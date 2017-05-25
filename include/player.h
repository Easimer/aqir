/*
 * player.h - CWowPlayer header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * This file is part of Aqir, see LICENSE
 */
#pragma once

#include <stdint.h>
#include <string>
#include <offsets.h>

namespace wow {
	class localplayer {
	public:
		static bool IsLooting(void);
		static bool IsMounted(void);
		static bool IsFishing(void);
		static bool IsInGame(void);
		static bool IsBobberBobbing(void);

		static float GetX(void);
		static float GetY(void);
		static float GetZ(void);

		static void SetX(float v);
		static void SetY(float v);
		static void SetZ(float v);

		static uint64_t getguid(void);
		static uint64_t getmguid(void);

		static unsigned _DEBUG_GetType(void);
		static struct wow_unit_desc* GetPlayerDescriptor();

		static std::string GetName(void);
		static int GetHealth(void);
		static int GetPower(void);
		static int GetMaxHealth(void);
		static int GetMaxPower(void);
		static int GetLevel(void);
	};
}