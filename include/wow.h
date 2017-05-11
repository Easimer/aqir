/*
 * wow.h - CWow header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#pragma once

#include <stdint.h>

class CWow {
public:
	static int ClntObjMgrGetActivePlayer(void);
	static unsigned short GetGameBuild(void);
	static uintptr_t GetPlayerPointer(void);
	static bool IsPlayerInGame(void);

private:
	const static uint64_t baseaddr = 0x140000000;
};

class CWowPlayer {
public:
	static bool IsMounted(void);
	static bool IsFishing(void);
	static bool IsInGame(void);
	static bool IsBobberBobbing(void);
};

struct WowUnit {
	uint64_t guid;
	uint32_t id;
	uint32_t flags1;
	uint64_t creator;
	uint32_t health;
	uint32_t power;
	uint32_t health_max;
	uint32_t power_max;
	uint32_t level;
	uint32_t flags2;
};

#define GAMEBUILD_OFF (0x1274FE4)
#define PLAYERPTR_OFF (0x141C2D0)

/* 
isBobbing
[ 0]       22a23c,  2 +        f823c,  misc, 0, [I32 I16 I8 ]
[ 1]       22a258,  2 +        f8258,  misc, 0, [I64 I32 I16 I8 ]
*/

#define PLAYER_ISMOUNTED (0x1EBC)
#define PLAYER_ISFISHING1 (0x14B1)
#define PLAYER_ISFISHING2 (0x1621) // possible candidate
#define PLAYER_ISFISHING3 (0x1627) // possible candidate
#define PLAYER_ISFISHING4 (0x6F41)
#define PLAYER_ISBOBBING1 (0x22a23c) // use this
#define PLAYER_ISBOBBING2 (0x22a258)
