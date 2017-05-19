/*
 * camera.h - Game camera header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#pragma once

#define CAMSTOFF 0x14A13B0
#define CAMOFF 0x85E8

#define CAMORIGOFF 0x10
#define CAMMATOFF 0x1C
#define CAMNPOFF 0x38
#define CAMFPOFF 0x3C
#define CAMFOVOFF 0x40
#define CAMAROFF 0x44

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
