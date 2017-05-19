/*
 * wow.cpp - Game-related functions
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#include <wow.h>
#include <atomic>

std::atomic<bool> ExecuteBot(false);

namespace wow {

	unsigned short game::GetGameBuild(void)
	{
		return *reinterpret_cast<unsigned short*>(BASEADDR + GAMEBUILD_OFF);
		//return *(unsigned short*)((baseaddr + GAMEBUILD_OFF));
	}

	uintptr_t game::GetPlayerPointer(void)
	{
		return *reinterpret_cast<uintptr_t*>(BASEADDR + PLAYERPTR_OFF);
		//return *(uintptr_t*)(baseaddr + PLAYERPTR_OFF);
	}

	void game::DisableBot(void)
	{
		ExecuteBot.store(false);
	}

	void game::EnableBot(void)
	{
		ExecuteBot.store(true);
	}

	bool game::IsBotEnabled(void)
	{
		return ExecuteBot.load();
	}

	uintptr_t game::GetObjMgrBase(void)
	{
		return *(uintptr_t*)(BASEADDR + 0xEC4628);
	}

	uintptr_t game::GetObjMgr(void)
	{
		return *(uintptr_t*)(GetObjMgrBase() + 0x462c);
	}

	uintptr_t game::GetObjMgrFirst(void)
	{
		return *(uintptr_t*)(GetObjMgr() + 0xCC);
	}

	uintptr_t game::GetEntityList(void)
	{
		return (uintptr_t)(BASEADDR + 0x1387048);
	}

	uintptr_t game::GetFirstEntityAddr(void)
	{
		return *(uintptr_t*)(GetEntityList() + 0x18);
	}

}
