/*
 * objmgr.h - WoW Object Manager header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * This file is part of Aqir, see LICENSE
 */

#pragma once

#include <cstdint>
#include <vector>
#include <offsets.h>

namespace wow {
	class objmgr {
	public:
		objmgr(void);
		uintptr_t GetObjectByName(const char* name);
		uintptr_t GetObjectByName(const char* name, unsigned type);
		uintptr_t GetOwnedObjectByName(const char* name, unsigned type);
		std::vector<uintptr_t> GetPlayers(void);
	private:
		uintptr_t base;
	};
}
