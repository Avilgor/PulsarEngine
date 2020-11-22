#ifndef __Camera_H__
#define __Camera_H__

#include "Component.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLib.h"

#pragma comment (lib, "MathGeoLib/libx86/MathGeoLib.lib") 


class Camera : public Component
{
public:

	Camera(GameObject* parent);
	~Camera();

	void UpdateComponent();
	void DeleteComponent();
	void SaveComponent(JSonHandler* file);
	void LoadComponent(JSonHandler* file);
	void UpdateCamera(const float4x4& global);
	void UpdateCameraPlanes();

	void LookAt(float3 pos);

	void SetNearPlane(float val);
	void SetFarPlane(float val);
	void SetFOV(float val);
	void SetAspectRatio(float val);
	void AdaptFOV(float width, float height);

	float* GetOpenGLViewMatrix();
	float* GetOpenGLProjectionMatrix();
	float GetNearPlane() { return frustum.NearPlaneDistance(); }
	float GetFarPlane() { return frustum.FarPlaneDistance(); }
	float GetFOV() { return frustum.VerticalFov() * RADTODEG; }
	float GetAspectRatio() { return frustum.AspectRatio(); }

public:
	Frustum frustum;
	bool culling = false;
	bool mainCamera = false;
	Plane planes[6];
};

#endif //__Camera_H__