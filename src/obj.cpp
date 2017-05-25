/*
 * obj.cpp - WoW Object class
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
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

	bool gameobj::isbobbing(void)
	{
		return *reinterpret_cast<bool*>(baseaddr + 376);
	}

	float gameobj::getx(void)
	{
		return *reinterpret_cast<float*>(baseaddr + 448);
	}

	float gameobj::gety(void)
	{
		return *reinterpret_cast<float*>(baseaddr + 452);
	}

	float gameobj::getz(void)
	{
		return *reinterpret_cast<float*>(baseaddr + 456);
	}

	void gameobj::setx(float v)
	{
		*reinterpret_cast<float*>(baseaddr + 448) = v;
	}
	
	void gameobj::sety(float v)
	{
		*reinterpret_cast<float*>(baseaddr + 452) = v;
	}

	void gameobj::setz(float v)
	{
		*reinterpret_cast<float*>(baseaddr + 456) = v;
	}

	uint64_t gameobj::getguid(void)
	{
		return *reinterpret_cast<uint64_t*>(*reinterpret_cast<uintptr_t*>(baseaddr + 0x08));
	}

	void gameobj::setmouse(void)
	{
		*reinterpret_cast<uint64_t*>(baseaddr + 0x14A20C0) = getguid();
	}

	player::player(void) : gameobj() {}
	player::player(uintptr_t& baseaddr) : gameobj(baseaddr) {}

	bool player::isbobbing(void)
	{
		return false;
	}

	float player::getx(void)
	{
		return *reinterpret_cast<float*>(baseaddr + 4160);
	}

	float player::gety(void)
	{
		return *reinterpret_cast<float*>(baseaddr + 4164);
	}

	float player::getz(void)
	{
		return *reinterpret_cast<float*>(baseaddr + 4168);
	}

	void player::setx(float v)
	{
		*reinterpret_cast<float*>(baseaddr + 4160) = v;
	}
	
	void player::sety(float v)
	{
		*reinterpret_cast<float*>(baseaddr + 4164) = v;
	}

	void player::setz(float v)
	{
		*reinterpret_cast<float*>(baseaddr + 4168) = v;
	}

	uint64_t player::getguid(void)
	{
		return *reinterpret_cast<uint64_t*>(*reinterpret_cast<uintptr_t*>(baseaddr + 0x08));
	}

	std::string player::name(void)
	{
		std::string pname = "(error)";
		if(!IsPointerValid(baseaddr + 824))
		{
			std::cerr << __LINE__ << std::endl;
			return pname;
		}
		uintptr_t cacheptr = *reinterpret_cast<uintptr_t*>(baseaddr + 824);
		/*if(!IsPointerValid(cacheptr))
		{
			std::cerr << __LINE__ << std::endl;
			return pname;
		}*/
		// that struct has a pointer @152 that points to the name string
		if(!IsPointerValid(cacheptr + 208))
		{
			std::cerr << __LINE__ << std::endl;
			return pname;
		}
		uintptr_t namestructptr = *reinterpret_cast<uintptr_t*>(cacheptr + 208);
		if(!IsPointerValid(namestructptr))
		{
			std::cerr << __LINE__ << std::endl;
			return pname;
		}
		pname = std::string((char*)namestructptr);
		return pname;
	}
}
