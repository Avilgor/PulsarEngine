#ifndef __Scene_H__
#define __Scene_H__

#include "Globals.h"
#include <string>

class GameObject;
class Mesh;
class Material;
class JSonHandler;

class Scene
{
public:

	Scene();
	Scene(const char* name);
	~Scene();

	void StartScene();
	update_status UpdateScene(float dt);
	void CleanScene();
	void SaveScene();
	void LoadScene(JSonHandler* file);
	void SaveTempScene();
	void LoadTempScene();
	GameObject* GetRoot() { return root; }
	GameObject* CreateEmptyGameobject();
	GameObject* CreateEmptyGameobject(const char* name);
	void CreateCube();
	void CreatePyramid();
	void CreatePlane();


public:
	GameObject* root = nullptr;
	std::string name = "UnknowScene";
};

#endif //__Scene_H__