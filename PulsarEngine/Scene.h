#ifndef __Scene_H__
#define __Scene_H__

#include "Globals.h"
#include "EngineResource.h"
#include <string>

class GameObject;
class Mesh;
class Material;
class JSonHandler;

class Scene : public EngineResource
{
public:

	Scene();
	Scene(const char* name);
	Scene(std::string uuid);
	Scene(const char* name, std::string uuid);
	~Scene();

	void StartScene();
	update_status UpdateScene(float dt);
	update_status PostUpdateScene(float dt);
	void Clean();
	void SaveResource(JSonHandler* file);
	void LoadResource(JSonHandler* file);
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


private:
	GameObject* root = nullptr;
};

#endif //__Scene_H__