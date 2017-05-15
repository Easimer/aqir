/*
 * obj.cpp - WoW Object class
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#include <obj.h>

CWowObject::CWowObject(uintptr_t baseaddr)
{
	this->baseaddr = baseaddr;
}

uint64_t CWowObject::GetGUID(void)
{
	return *reinterpret_cast<uint64_t*>(baseaddr + 40);
}
