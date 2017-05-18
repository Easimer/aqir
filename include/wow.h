/*
 * wow.h - CWow header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#pragma once

#include <stdint.h>
#include <obj.h>
#include <string>

class CWow {
public:
	static int ClntObjMgrGetActivePlayer(void);
	
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

private:
	const static uint64_t baseaddr = 0x140000000;
};

class CWowPlayer {
public:
	static bool IsMounted(void);
	static bool IsFishing(void);
	static bool IsInGame(void);
	static bool IsBobberBobbing(void);

	static int GetX(void);
	static int GetY(void);
	static int GetZ(void);

	static void SetX(int v);
	static void SetY(int v);
	static void SetZ(int v);

	static unsigned _DEBUG_GetType(void);
	static struct wow_unit_desc* GetPlayerDescriptor();

	static std::string GetName(void);
	static int GetHealth(void);
	static int GetPower(void);
	static int GetMaxHealth(void);
	static int GetMaxPower(void);
	static int GetLevel(void);
};

#define GAMEBUILD_OFF (0x1274FE4)
#define PLAYERPTR_OFF (0x141C2D0)

#define PLAYER_ISMOUNTED (0x1EBC)
#define PLAYER_ISFISHING1 (0x1621) // possible candidate
#define PLAYER_ISFISHING2 (0x1627) // possible candidate
#define PLAYER_ISBOBBING1 (0x22a23c)
#define PLAYER_ISBOBBING2 (0x22a258)
#define PLAYER_ISLOOTING (0x15121C4)
#define PLAYER_X (0x1040)
#define PLAYER_Y (0x1044)
#define PLAYER_Z (0x1048)

#define PLAYER_CACHE (4592)
#define PLAYER_CACHE_NAME (152)

#define BASEADDR (0x140000000)
#define CLIENTCONN (0x009BE678)
#define OBJMGR (0x463C)
#define OBJMGR_0 (0x18)
#define OBJMGR_N (0x58)
#define ENTLIST (0x1387048)
