#ifndef __Scene_H__
#define __Scene_H__

#include "Globals.h"
#include "EngineResource.h"
#include <string>
#include <vector>

class GameObject;
class Mesh;
class Material;
class JSonHandler;
struct PhysVehicle3D;


class Scene : public EngineResource
{
public:

	Scene();
	Scene(const char* name);
	Scene(std::string uuid);
	Scene(const char* name, std::string uuid);
	~Scene();

	void StartScene();
	update_status PreUpdateScene(float dt);
	update_status UpdateScene(float dt);
	update_status PostUpdateScene(float dt);
	void Clean();
	void SaveResource(JSonHandler* file);
	void LoadResource(JSonHandler* file);
	bool LoadScene(JSonHandler* node);
	void SaveTempScene();
	void LoadTempScene();
	GameObject* GetRoot() { return root; }
	GameObject* CreateEmptyGameobject();
	GameObject* CreateEmptyGameobject(const char* name);
	void CreateCube();
	void CreatePyramid();
	void CreateSphere();
	void CreateCylinder();
	void CreateTorus();
	void CreatePlane();
	void ClearBalls();
	void StartRecoveringScene();
	//void SetName(std::string n);

private:
	void ThrowPhysBall();	
	void AddCar();

private:
	GameObject* root = nullptr;
	PhysVehicle3D* vehicle = nullptr;
	float turn;
	float acceleration;
	float brake;
	std::vector<GameObject*> balls;
	GameObject* cameraCollider = nullptr;
	bool recoveringScene = false;
	int recoverSteps = 0;
};

#endif //__Scene_H__