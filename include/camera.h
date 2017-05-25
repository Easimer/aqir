/*
 * camera.h - Game camera header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * This file is part of Aqir, see LICENSE
 */
#pragma once

#include <offsets.h>

#define M_DEG2RAD 0.017453292519943295769236907684886f

namespace wow {
	class camera {
	public:
		static void GetMatrix(float* out);
		static void GetPosition(float* out);
		static float GetFieldOfView(void);
		static float GetNearPlane(void);
		static float GetFarPlane(void);
		static float GetAspectRatio(void);

		static void WorldToScreen(float* in, float* out);
	};
}
