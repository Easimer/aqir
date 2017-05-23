/*
 * target.h - Target header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#pragma once

#include <stdint.h>

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

#define TARGETOFF 0x14A20D8
