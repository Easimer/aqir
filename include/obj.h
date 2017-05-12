/*
 * obj.h - WoW Object header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#pragma once

#include <wow.h>
#include <stdint.h>

class CWowObject {
public:
	CWowObject(uintptr_t baseaddr);
	uint64_t GetGUID(void);
private:
	uintptr_t baseaddr;
};
