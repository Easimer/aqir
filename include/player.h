/*
 * player.h - CWowPlayer header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#pragma once

#include <stdint.h>
#include <string>

#define PLAYER_CACHE (4592)
#define PLAYER_CACHE_NAME (152)

#define PLAYER_ISMOUNTED (0x1EBC)
#define PLAYER_ISFISHING1 (0x1621) // possible candidate
#define PLAYER_ISFISHING2 (0x1627) // possible candidate
#define PLAYER_ISBOBBING1 (0x22a23c)
#define PLAYER_ISBOBBING2 (0x22a258)
#define PLAYER_ISLOOTING (0x15121C4)

#define PLAYER_X (0x1040)
#define PLAYER_Y (0x1044)
#define PLAYER_Z (0x1048)

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