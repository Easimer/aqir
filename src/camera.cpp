/*
 * camera.cpp - Game camera functions
 * Author: Daniel Meszaros <easimer@gmail.com>
 * This file is part of Aqir, see LICENSE
 */
#include <wow.h>
#include <camera.h>
#include <cstring>
#include <utils.h>
#include <cmath>

namespace wow {
	// Copies the local camera's viewmatrix to out
	void camera::GetMatrix(float* out)
	{
		uintptr_t CameraPtr = BASEADDR + CAMSTOFF;
		uintptr_t ptr = *reinterpret_cast<uintptr_t*>(CameraPtr);
		uintptr_t CameraBase = *reinterpret_cast<uintptr_t*>(ptr + CAMOFF);
		float* CameraMatrix = reinterpret_cast<float*>(CameraBase + CAMMATOFF);
		memcpy(out, CameraMatrix, 36);
	}

	// Copies the local camera's position to out
	void camera::GetPosition(float* out)
	{
		uintptr_t CameraPtr = BASEADDR + CAMSTOFF;
		uintptr_t ptr = *reinterpret_cast<uintptr_t*>(CameraPtr);
		uintptr_t CameraBase = *reinterpret_cast<uintptr_t*>(ptr + CAMOFF);
		float* CameraOrigin = reinterpret_cast<float*>(CameraBase + CAMORIGOFF);
		memcpy(out, CameraOrigin, 3 * sizeof(float));
	}

	// Returns the field of view
	float camera::GetFieldOfView(void)
	{
		uintptr_t CameraPtr = BASEADDR + CAMSTOFF;
		uintptr_t ptr = *reinterpret_cast<uintptr_t*>(CameraPtr);
		uintptr_t CameraBase = *reinterpret_cast<uintptr_t*>(ptr + CAMOFF);
		return *reinterpret_cast<float*>(CameraBase + CAMFOVOFF);
	}

	// Returns the camera near plane distance
	float camera::GetNearPlane(void)
	{
		uintptr_t CameraPtr = BASEADDR + CAMSTOFF;
		uintptr_t ptr = *reinterpret_cast<uintptr_t*>(CameraPtr);
		uintptr_t CameraBase = *reinterpret_cast<uintptr_t*>(ptr + CAMOFF);
		return *reinterpret_cast<float*>(CameraBase + CAMNPOFF);
	}

	// Returns the camera far plane distance
	float camera::GetFarPlane(void)
	{
		uintptr_t CameraPtr = BASEADDR + CAMSTOFF;
		uintptr_t ptr = *reinterpret_cast<uintptr_t*>(CameraPtr);
		uintptr_t CameraBase = *reinterpret_cast<uintptr_t*>(ptr + CAMOFF);
		return *reinterpret_cast<float*>(CameraBase + CAMFPOFF);
	}

	// Returns the aspect ratio
	float camera::GetAspectRatio(void)
	{
		uintptr_t CameraPtr = BASEADDR + CAMSTOFF;
		uintptr_t ptr = *reinterpret_cast<uintptr_t*>(CameraPtr);
		uintptr_t CameraBase = *reinterpret_cast<uintptr_t*>(ptr + CAMOFF);
		return *reinterpret_cast<float*>(CameraBase + CAMAROFF);
	}

	// Project vector3 at 'in' onto the screen, return vector2 to 'out'
	void camera::WorldToScreen(float* in, float* out)
	{
		float fDiff[3];
		float fProd;
		float fInv[3][3];
		float fDet, fInvDet;
		float fCam[3];
		float fView[3];
		float fScreenX, fScreenY;
		float fTmpX, fTmpY;

		float fViewMat[3][3];
		float fCamPos[3];
		float fFov;

		GetMatrix((float*)fViewMat);
		GetPosition(fCamPos);
		fFov = GetFieldOfView();

		float x = in[0];
		float y = in[1];
		float z = in[2];

		out[0] = -1;
		out[1] = -1;

		fDiff[0] = x-fCamPos[0];
		fDiff[1] = y-fCamPos[1];
		fDiff[2] = z-fCamPos[2];

		//float fProd = vDiff*camera.matView[0];
		fProd = 
			fDiff[0]*fViewMat[0][0] +
			fDiff[1]*fViewMat[0][1] +
			fDiff[2]*fViewMat[0][2];
		if( fProd < 0 )
			return;

		//CVec3 vView = vDiff*!camera.matView;
		fInv[0][0] = fViewMat[1][1]*fViewMat[2][2]-fViewMat[1][2]*fViewMat[2][1];
		fInv[1][0] = fViewMat[1][2]*fViewMat[2][0]-fViewMat[1][0]*fViewMat[2][2];
		fInv[2][0] = fViewMat[1][0]*fViewMat[2][1]-fViewMat[1][1]*fViewMat[2][0];
		fDet = fViewMat[0][0]*fInv[0][0]+fViewMat[0][1]*fInv[1][0]+fViewMat[0][2]*fInv[2][0];
		fInvDet = 1.0f / fDet;
		fInv[0][1] = fViewMat[0][2]*fViewMat[2][1]-fViewMat[0][1]*fViewMat[2][2];
		fInv[0][2] = fViewMat[0][1]*fViewMat[1][2]-fViewMat[0][2]*fViewMat[1][1];
		fInv[1][1] = fViewMat[0][0]*fViewMat[2][2]-fViewMat[0][2]*fViewMat[2][0];
		fInv[1][2] = fViewMat[0][2]*fViewMat[1][0]-fViewMat[0][0]*fViewMat[1][2];
		fInv[2][1] = fViewMat[0][1]*fViewMat[2][0]-fViewMat[0][0]*fViewMat[2][1];
		fInv[2][2] = fViewMat[0][0]*fViewMat[1][1]-fViewMat[0][1]*fViewMat[1][0];
		fViewMat[0][0] = fInv[0][0]*fInvDet;
		fViewMat[0][1] = fInv[0][1]*fInvDet;
		fViewMat[0][2] = fInv[0][2]*fInvDet;
		fViewMat[1][0] = fInv[1][0]*fInvDet;
		fViewMat[1][1] = fInv[1][1]*fInvDet;
		fViewMat[1][2] = fInv[1][2]*fInvDet;
		fViewMat[2][0] = fInv[2][0]*fInvDet;
		fViewMat[2][1] = fInv[2][1]*fInvDet;
		fViewMat[2][2] = fInv[2][2]*fInvDet;
		
		fView[0] = fInv[0][0]*fDiff[0]+fInv[1][0]*fDiff[1]+fInv[2][0]*fDiff[2];
		fView[1] = fInv[0][1]*fDiff[0]+fInv[1][1]*fDiff[1]+fInv[2][1]*fDiff[2];
		fView[2] = fInv[0][2]*fDiff[0]+fInv[1][2]*fDiff[1]+fInv[2][2]*fDiff[2];

		//CVec3 vCam( -vView.fY,-vView.fZ,vView.fX );
		fCam[0] = -fView[1];
		fCam[1] = -fView[2];
		fCam[2] =  fView[0];

		fScreenX = (1920)/2.0f;
		fScreenY = (1080)/2.0f;
		// Thanks pat0! Aspect ratio fix
		fTmpX    = fScreenX/tan(((fFov*44.0f)/2.0f)*M_DEG2RAD);
		fTmpY    = fScreenY/tan(((fFov*35.0f)/2.0f)*M_DEG2RAD);

		out[0] = fScreenX + fCam[0]*fTmpX/fCam[2];
		out[1] = fScreenY + fCam[1]*fTmpY/fCam[2];
	}

}
