#ifndef __ModuleCamera3D_H__
#define __ModuleCamera3D_H__

#include "Module.h"
#include "Globals.h"
#include "Camera.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLib.h"

#pragma comment (lib, "MathGeoLib/libx86/MathGeoLib.lib") 

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);

	bool CleanUp();

	void Look(float3 Position);
	void Move(vec val);
	void Zoom(float val);
	void Pan(float x,float y);
	void SetReference(float3 pos);
	float3 GetPos();
	float* GetGLViewMatrix();
	float* GetGLProjectionMatrix();
	AABBCheck CheckAABB(AABB box);
	LineSegment CastRay(float x,float y);
	void AddDrawnGameobject(GameObject* go);
	std::vector<GameObject*> GetDrawnObjects() { return drawnGameobjects; }
	void ResizeView(int w, int h);
	void SetMainCamera(Camera* cam);
	float3 GetEditorDirection() { return camera->GetDirection(); }
	float3 GetMainrDirection() { return mainCamera->GetDirection(); }

public:
	Camera* camera = nullptr;
	Camera* mainCamera = nullptr;
private:
	bool lockRef;
	float3 reference;
	bool mouseDrag;
	std::vector<GameObject*> drawnGameobjects;
};

#endif //__ModuleCamera3D_H__