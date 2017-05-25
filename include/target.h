/*
 * target.h - Target header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * This file is part of Aqir, see LICENSE
 */
#pragma once

#include <stdint.h>
#include <offsets.h>

namespace wow {
	class target {
	public:
		static uintptr_t getptr(void);

		static float getx(void);
		static float gety(void);
		static float getz(void);
		
		static float dist(void);
		static float dist(uintptr_t other);
		
		static int gethealth(void);
		static int getpower(void);
		static int getmaxhealth(void);
		static int getmaxpower(void);
		static int getlevel(void);
	};
}
