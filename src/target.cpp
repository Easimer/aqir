/*
 * target.h - Target class
 * Author: Daniel Meszaros <easimer@gmail.com>
 * This file is part of Aqir, see LICENSE
 */
#include <target.h>

namespace wow {
	uintptr_t target::getptr(void)
	{
		return *reinterpret_cast<uintptr_t*>(TARGETOFF);
	}

	float target::getx(void)
	{
		
	}
}
