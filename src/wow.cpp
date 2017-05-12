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
	return 0;
	return ((int (*)(void))(baseaddr + 0x4FC6))();
}

unsigned short CWow::GetGameBuild(void)
{
	return *(unsigned short*)((baseaddr + GAMEBUILD_OFF));
}

uintptr_t CWow::GetPlayerPointer(void)
{
	return *(uintptr_t*)(baseaddr + PLAYERPTR_OFF);
}

bool CWow::IsPlayerLooting(void)
{
	return *(uint8_t*)(baseaddr + 0x15121C4);
}

bool CWowPlayer::IsInGame(void)
{
	return CWow::GetPlayerPointer() != 0;
}

bool CWowPlayer::IsMounted(void)
{
	uintptr_t pbase = CWow::GetPlayerPointer();
	uint8_t* m = (uint8_t*)(pbase + PLAYER_ISMOUNTED);
	return (*m);
}

bool CWowPlayer::IsFishing(void)
{
	uintptr_t pbase = CWow::GetPlayerPointer();
	uint8_t* f2 = (uint8_t*)(pbase + PLAYER_ISFISHING2);
	uint8_t* f3 = (uint8_t*)(pbase + PLAYER_ISFISHING3);
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
