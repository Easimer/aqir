/*
 * obj.cpp - WoW Object class
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#include <obj.h>
#include <utils.h>

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
}
