/*
 * player.cpp - player
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#include <wow.h>
#include <player.h>

namespace wow {

	bool localplayer::IsLooting(void)
	{
		return *reinterpret_cast<uint8_t*>(BASEADDR + PLAYER_ISLOOTING);
		//return *(uint8_t*)(baseaddr + PLAYER_ISLOOTING);
	}

	bool localplayer::IsInGame(void)
	{
		return game::GetPlayerPointer() != 0;
	}

	bool localplayer::IsMounted(void)
	{
		//uintptr_t pbase = game::GetPlayerPointer();
		//uint8_t* m = (uint8_t*)(pbase + PLAYER_ISMOUNTED);
		//return (*m);

		return *reinterpret_cast<uint8_t*>(game::GetPlayerPointer() + PLAYER_ISMOUNTED);
	}

	bool localplayer::IsFishing(void)
	{
		uintptr_t pbase = game::GetPlayerPointer();
		uint8_t* f2 = (uint8_t*)(pbase + PLAYER_ISFISHING1);
		uint8_t* f3 = (uint8_t*)(pbase + PLAYER_ISFISHING2);
		return (*f2 && *f3);
	}

	bool localplayer::IsBobberBobbing(void)
	{
		uint32_t* bb1 = (uint32_t*)PLAYER_ISBOBBING1;
		return *bb1;
	}

	unsigned localplayer::_DEBUG_GetType(void)
	{
		uintptr_t pbase = game::GetPlayerPointer();
		uint32_t* p = reinterpret_cast<uint32_t*>(pbase + 24);
		return *p;
	}

	float localplayer::GetX(void)
	{
		uintptr_t pbase = game::GetPlayerPointer();
		float* p = reinterpret_cast<float*>(pbase + PLAYER_X);
		return *p;
	}

	float localplayer::GetY(void)
	{
		uintptr_t pbase = game::GetPlayerPointer();
		float* p = reinterpret_cast<float*>(pbase + PLAYER_Y);
		return *p;
	}
	float localplayer::GetZ(void)
	{
		uintptr_t pbase = game::GetPlayerPointer();
		float* p = reinterpret_cast<float*>(pbase + PLAYER_Z);
		return *p;
	}

	void localplayer::SetX(float v)
	{
		*reinterpret_cast<float*>(game::GetPlayerPointer() + PLAYER_X) = v;
	}

	void localplayer::SetY(float v)
	{
		*reinterpret_cast<float*>(game::GetPlayerPointer() + PLAYER_Y) = v;
	}

	void localplayer::SetZ(float v)
	{
		*reinterpret_cast<float*>(game::GetPlayerPointer() + PLAYER_Z) = v;
	}

	struct wow_unit_desc* localplayer::GetPlayerDescriptor(void)
	{
		uintptr_t pbase = game::GetPlayerPointer();
		uintptr_t* ptr2desc = (uintptr_t*)(pbase + 8); // ptr 2 
		return (struct wow_unit_desc*)*ptr2desc;
	}

	int localplayer::GetHealth(void)
	{
		return GetPlayerDescriptor()->health;
	}

	int localplayer::GetPower(void)
	{
		return GetPlayerDescriptor()->power;
	}

	int localplayer::GetMaxHealth(void)
	{
		return GetPlayerDescriptor()->health_max;
	}

	int localplayer::GetMaxPower(void)
	{
		return GetPlayerDescriptor()->power_max;
	}

	int localplayer::GetLevel(void)
	{
		return GetPlayerDescriptor()->level;
	}

	std::string localplayer::GetName(void)
	{
		// there's a pointer in the player desc @4592 that points to a cache structure.
		uintptr_t cacheptr = *reinterpret_cast<uintptr_t*>(game::GetPlayerPointer() + PLAYER_CACHE);
		// that struct has a pointer @152 that points to the name string
		uintptr_t namestructptr = *reinterpret_cast<uintptr_t*>(cacheptr + PLAYER_CACHE_NAME);
		const char* name = (const char*)(namestructptr);
		return std::string(name);
	}

	uint64_t localplayer::getguid(void)
	{
		return GetPlayerDescriptor()->guid;
	}

	uint64_t localplayer::getmguid(void)
	{
		return *reinterpret_cast<uint64_t*>(0x1414A20C0);
	}

}
