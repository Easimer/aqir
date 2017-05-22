/*
 * obj.cpp - WoW Object class
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */

#include <objmgr.h>
#include <cstring>
#include <utils.h>
#include <player.h>
#include <obj.h>

namespace wow {
	objmgr::objmgr(void)
	{
		base = BASEADDR + ENTLIST;
	}

	uintptr_t objmgr::GetObjectByName(const char* name)
	{
		return GetObjectByName(name, 0xFF);
	}

	uintptr_t objmgr::GetObjectByName(const char* name, unsigned type)
	{
		uintptr_t EntityList = *reinterpret_cast<uintptr_t*>(base);
		uintptr_t FirstEntity = *reinterpret_cast<uintptr_t*>(EntityList + OBJMGR_0);
		uintptr_t EntityPtr = FirstEntity;
		while((EntityPtr != 0) && ((EntityPtr & 1) == 0) && IsPointerValid(EntityPtr))
		{
			uintptr_t cache;
			char* oname;
			uint64_t otype = READ_UINT64(EntityPtr + 0x18);
			if((otype != type && type != 0xFF)/* || otype > O_CORPSE*/)
				goto cont;
			cache = *reinterpret_cast<uintptr_t*>(EntityPtr + 824);
			if(!IsPointerValid(cache + 208))
				goto cont;
			oname = *reinterpret_cast<char**>(cache + 208);

			if(!IsPointerValid((uintptr_t)oname))
				goto cont;
			if(strncmp(name, oname, 30) == 0)
				return EntityPtr;

			cont:
			if(IsPointerValid(EntityPtr + OBJMGR_N))
				EntityPtr = *reinterpret_cast<uintptr_t*>(EntityPtr + OBJMGR_N);
			else
				return 0;
		}
		return 0;
	}

	uintptr_t objmgr::GetOwnedObjectByName(const char* name, unsigned type)
	{
		uint64_t PlayerGUID = wow::localplayer::getguid();
		uintptr_t EntityList = *reinterpret_cast<uintptr_t*>(base);
		uintptr_t FirstEntity = *reinterpret_cast<uintptr_t*>(EntityList + OBJMGR_0);
		uintptr_t EntityPtr = FirstEntity;
		while((EntityPtr != 0) && ((EntityPtr & 1) == 0) && IsPointerValid(EntityPtr))
		{
			uintptr_t cache;
			char* oname;
			uint64_t otype;
			uint64_t ocguid = *reinterpret_cast<uint64_t*>(*reinterpret_cast<uintptr_t*>(EntityPtr + 0x08) + 0x20);

			if(ocguid != PlayerGUID)
				goto cont;

			otype = READ_UINT64(EntityPtr + 0x18);
			if((otype != type && type != 0xFF)/* || otype > O_CORPSE*/)
				goto cont;
			cache = *reinterpret_cast<uintptr_t*>(EntityPtr + 824);
			if(!IsPointerValid(cache + 208))
				goto cont;
			oname = *reinterpret_cast<char**>(cache + 208);

			if(!IsPointerValid((uintptr_t)oname))
				goto cont;
			if(strncmp(name, oname, 30) == 0)
				return EntityPtr;

			cont:
			if(IsPointerValid(EntityPtr + OBJMGR_N))
				EntityPtr = *reinterpret_cast<uintptr_t*>(EntityPtr + OBJMGR_N);
			else
				return 0;
		}
		return 0;
	}

	std::vector<uintptr_t> objmgr::GetPlayers(void)
	{
		std::vector<uintptr_t> players;

		uintptr_t EntityList = *reinterpret_cast<uintptr_t*>(base);
		uintptr_t FirstEntity = *reinterpret_cast<uintptr_t*>(EntityList + OBJMGR_0);
		uintptr_t EntityPtr = FirstEntity;
		while((EntityPtr != 0) && ((EntityPtr & 1) == 0) && IsPointerValid(EntityPtr))
		{
			uint64_t type = READ_UINT64(EntityPtr + 0x18);
			if(type != O_PLAYER)
				goto cont;

			players.push_back(EntityPtr);

			cont:
			if(IsPointerValid(EntityPtr + OBJMGR_N))
				EntityPtr = *reinterpret_cast<uintptr_t*>(EntityPtr + OBJMGR_N);
			else
				break;
		}
		return players;
	}
}
