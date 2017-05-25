/*
 * obj.cpp - WoW Object class
 * Author: Daniel Meszaros <easimer@gmail.com>
 * This file is part of Aqir, see LICENSE
 */
#include <obj.h>
#include <utils.h>
#include <iostream>

namespace wow {

	obj::obj(void)
	{
		baseaddr = 0;
	}

	obj::obj(uintptr_t& baseaddr)
	{
		this->baseaddr = baseaddr;
	}

	void obj::setbase(uintptr_t addr)
	{
		this->baseaddr = addr;
	}

	gameobj::gameobj(void) : obj() {}
	gameobj::gameobj(uintptr_t& baseaddr) : obj(baseaddr) {}

	// Is bobbing. Like a Fishing Bobber
	bool gameobj::isbobbing(void)
	{
		return *reinterpret_cast<bool*>(baseaddr + GOBJ_OFF_BOBBING);
	}

	float gameobj::getx(void)
	{
		return *reinterpret_cast<float*>(baseaddr + GOBJ_OFF_POSX);
	}

	float gameobj::gety(void)
	{
		return *reinterpret_cast<float*>(baseaddr + GOBJ_OFF_POSY);
	}

	float gameobj::getz(void)
	{
		return *reinterpret_cast<float*>(baseaddr + GOBJ_OFF_POSZ);
	}

	// These usually don't work
	void gameobj::setx(float v)
	{
		*reinterpret_cast<float*>(baseaddr + GOBJ_OFF_POSX) = v;
	}
	
	void gameobj::sety(float v)
	{
		*reinterpret_cast<float*>(baseaddr + GOBJ_OFF_POSY) = v;
	}

	void gameobj::setz(float v)
	{
		*reinterpret_cast<float*>(baseaddr + GOBJ_OFF_POSZ) = v;
	}

	uint64_t gameobj::getguid(void)
	{
		return *reinterpret_cast<uint64_t*>(*reinterpret_cast<uintptr_t*>(baseaddr + GOBJ_OFF_DESC) + GOBJ_OFF_DESC_GUID);
	}

	// Sets the Mouseover GUID to this object's GUID
	void gameobj::setmouse(void)
	{
		*reinterpret_cast<uint64_t*>(baseaddr + MOUSE_GUID) = getguid();
	}

	player::player(void) : gameobj() {}
	player::player(uintptr_t& baseaddr) : gameobj(baseaddr) {}

	bool player::isbobbing(void)
	{
		return false;
	}

	float player::getx(void)
	{
		return *reinterpret_cast<float*>(baseaddr + PLAYER_X);
	}

	float player::gety(void)
	{
		return *reinterpret_cast<float*>(baseaddr + PLAYER_Y);
	}

	float player::getz(void)
	{
		return *reinterpret_cast<float*>(baseaddr + PLAYER_Z);
	}

	// These only work on the local player
	void player::setx(float v)
	{
		*reinterpret_cast<float*>(baseaddr + PLAYER_X) = v;
	}
	
	void player::sety(float v)
	{
		*reinterpret_cast<float*>(baseaddr + PLAYER_Y) = v;
	}

	void player::setz(float v)
	{
		*reinterpret_cast<float*>(baseaddr + PLAYER_Z) = v;
	}

	uint64_t player::getguid(void)
	{
		return *reinterpret_cast<uint64_t*>(*reinterpret_cast<uintptr_t*>(baseaddr + UNIT_OFF_DESC) + UNIT_OFF_DESC_GUID);
	}

	// Broken, TODO: implement NPC Cache enumeration method for this
	std::string player::name(void)
	{
		std::string pname = "(error)";
		if(!IsPointerValid(baseaddr + PLAYER_CACHE))
		{
			std::cerr << __LINE__ << std::endl;
			return pname;
		}
		uintptr_t cacheptr = *reinterpret_cast<uintptr_t*>(baseaddr + PLAYER_CACHE);
		/*if(!IsPointerValid(cacheptr))
		{
			std::cerr << __LINE__ << std::endl;
			return pname;
		}*/
		// that struct has a pointer @152 that points to the name string
		if(!IsPointerValid(cacheptr + PLAYER_CACHE_NAME))
		{
			std::cerr << __LINE__ << std::endl;
			return pname;
		}
		uintptr_t namestructptr = *reinterpret_cast<uintptr_t*>(cacheptr + PLAYER_CACHE_NAME);
		if(!IsPointerValid(namestructptr))
		{
			std::cerr << __LINE__ << std::endl;
			return pname;
		}
		pname = std::string((char*)namestructptr);
		return pname;
	}
}
