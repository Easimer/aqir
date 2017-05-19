/*
 * objmgr.h - WoW Object Manager header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */

#pragma once

#include <cstdint>

#define BASEADDR (0x140000000)
#define ENTLIST (0x1387048)
#define OBJMGR_0 (0x18)
#define OBJMGR_N (0x58)

namespace wow {
	class objmgr {
	public:
		objmgr(void);
		uintptr_t GetObjectByName(const char* name);
		uintptr_t GetObjectByName(const char* name, unsigned type);
		uintptr_t GetOwnedObjectByName(const char* name, unsigned type);
	private:
		uintptr_t base;
	};
}
