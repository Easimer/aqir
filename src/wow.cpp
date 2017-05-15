/*
 * wow.cpp - Game-related functions
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#include <wow.h>
#include <atomic>

std::atomic<bool> ExecuteBot(false);

int CWow::ClntObjMgrGetActivePlayer(void)
{
	return reinterpret_cast<int (*)(void)>(baseaddr + 0x4FC6)();
	//return ((int (*)(void))(baseaddr + 0x4FC6))();
}

unsigned short CWow::GetGameBuild(void)
{
	return *reinterpret_cast<unsigned short*>(baseaddr + GAMEBUILD_OFF);
	//return *(unsigned short*)((baseaddr + GAMEBUILD_OFF));
}

uintptr_t CWow::GetPlayerPointer(void)
{
	return *reinterpret_cast<uintptr_t*>(baseaddr + PLAYERPTR_OFF);
	//return *(uintptr_t*)(baseaddr + PLAYERPTR_OFF);
}

bool CWow::IsPlayerLooting(void)
{
	return *reinterpret_cast<uint8_t*>(baseaddr + PLAYER_ISLOOTING);
	//return *(uint8_t*)(baseaddr + PLAYER_ISLOOTING);
}

bool CWowPlayer::IsInGame(void)
{
	return CWow::GetPlayerPointer() != 0;
}

bool CWowPlayer::IsMounted(void)
{
	//uintptr_t pbase = CWow::GetPlayerPointer();
	//uint8_t* m = (uint8_t*)(pbase + PLAYER_ISMOUNTED);
	//return (*m);

	return *reinterpret_cast<uint8_t*>(CWow::GetPlayerPointer() + PLAYER_ISMOUNTED);
}

bool CWowPlayer::IsFishing(void)
{
	uintptr_t pbase = CWow::GetPlayerPointer();
	uint8_t* f2 = (uint8_t*)(pbase + PLAYER_ISFISHING1);
	uint8_t* f3 = (uint8_t*)(pbase + PLAYER_ISFISHING2);
	return (*f2 && *f3);
}

bool CWowPlayer::IsBobberBobbing(void)
{
	uint32_t* bb1 = (uint32_t*)PLAYER_ISBOBBING1;
	return *bb1;
}

void CWow::DisableBot(void)
{
	ExecuteBot.store(false);
}

void CWow::EnableBot(void)
{
	ExecuteBot.store(true);
}

bool CWow::IsBotEnabled(void)
{
	return ExecuteBot.load();
}

uintptr_t CWow::GetObjMgrBase(void)
{
	return *(uintptr_t*)(baseaddr + 0xEC4628);
}

uintptr_t CWow::GetObjMgr(void)
{
	return *(uintptr_t*)(GetObjMgrBase() + 0x462c);
}

uintptr_t CWow::GetObjMgrFirst(void)
{
	return *(uintptr_t*)(GetObjMgr() + 0xCC);
}

uintptr_t CWow::GetEntityList(void)
{
	return (uintptr_t)(baseaddr + 0x1387048);
}

uintptr_t CWow::GetFirstEntityAddr(void)
{
	return *(uintptr_t*)(GetEntityList() + 0x18);
}

unsigned CWowPlayer::_DEBUG_GetType(void)
{
	uintptr_t pbase = CWow::GetPlayerPointer();
	uint32_t* p = reinterpret_cast<uint32_t*>(pbase + 24);
	return *p;
}

int CWowPlayer::GetX(void)
{
	uintptr_t pbase = CWow::GetPlayerPointer();
	int32_t* p = reinterpret_cast<int32_t*>(pbase + PLAYER_X);
	return *p;
}

int CWowPlayer::GetY(void)
{
	uintptr_t pbase = CWow::GetPlayerPointer();
	int32_t* p = reinterpret_cast<int32_t*>(pbase + PLAYER_Y);
	return *p;
}
int CWowPlayer::GetZ(void)
{
	uintptr_t pbase = CWow::GetPlayerPointer();
	int32_t* p = reinterpret_cast<int32_t*>(pbase + PLAYER_Z);
	return *p;
}

void CWowPlayer::SetX(int v)
{
	*reinterpret_cast<uint32_t*>(CWow::GetPlayerPointer() + PLAYER_X) = v;
}

void CWowPlayer::SetY(int v)
{
	*reinterpret_cast<uint32_t*>(CWow::GetPlayerPointer() + PLAYER_Y) = v;
}

void CWowPlayer::SetZ(int v)
{
	*reinterpret_cast<uint32_t*>(CWow::GetPlayerPointer() + PLAYER_Z) = v;
}

struct wow_unit_desc* CWowPlayer::GetPlayerDescriptor(void)
{
	uintptr_t pbase = CWow::GetPlayerPointer();
	uintptr_t* ptr2desc = (uintptr_t*)(pbase + 8); // ptr 2 
	return (struct wow_unit_desc*)*ptr2desc;
}

int CWowPlayer::GetHealth(void)
{
	return GetPlayerDescriptor()->health;
}

int CWowPlayer::GetPower(void)
{
	return GetPlayerDescriptor()->power;
}

int CWowPlayer::GetMaxHealth(void)
{
	return GetPlayerDescriptor()->health_max;
}

int CWowPlayer::GetMaxPower(void)
{
	return GetPlayerDescriptor()->power_max;
}

int CWowPlayer::GetLevel(void)
{
	return GetPlayerDescriptor()->level;
}

std::string CWowPlayer::GetName(void)
{
	// there's a pointer in the player desc @4592 that points to a cache structure.
	uintptr_t cacheptr = *reinterpret_cast<uintptr_t*>(CWow::GetPlayerPointer() + PLAYER_CACHE);
	// that struct has a pointer @152 that points to the name string
	uintptr_t namestructptr = *reinterpret_cast<uintptr_t*>(cacheptr + PLAYER_CACHE_NAME);
	const char* name = (const char*)(namestructptr);
	return std::string(name);
}
