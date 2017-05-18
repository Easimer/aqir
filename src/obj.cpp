/*
 * obj.cpp - WoW Object class
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#include <obj.h>
#include <utils.h>
#include <cstring>

CWowObject::CWowObject(uintptr_t baseaddr)
{
	this->baseaddr = baseaddr;
}

uint64_t CWowObject::GetGUID(void)
{
	return *reinterpret_cast<uint64_t*>(baseaddr + 40);
}

#include <iostream>

namespace wow {

objmgr::objmgr(void)
{
	base = BASEADDR + ENTLIST;
}

uintptr_t objmgr::GetObjectByName(const char* name)
{
	return GetObjectByName(name, 0xFF);
}

uintptr_t objmgr::GetObjectByName(const char* name, uint type)
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
	
}
