#ifndef __FileManager_H__
#define __FileManager_H__

#include "Module.h"

#include <string>
#include <map>

class RES_Material;
class RES_Mesh;
class Scene;

class FileManager : public Module
{
public:
	FileManager(Application* app, bool start_enabled = true);
	~FileManager();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void SaveMeshResource(RES_Mesh* mesh);
	void SaveMaterialResource(RES_Material* mat);
	void SaveSceneResource(Scene* scene);

	RES_Mesh* GetMeshResource(std::string uuid);
	RES_Material* GetMaterialResource(std::string uuid);
	Scene* GetSceneResource(std::string uuid);

private:
	std::map<std::string, RES_Material*> materialResources;
	std::map<std::string, RES_Mesh*> meshResources;
	std::map<std::string, Scene*> sceneResources;
};

#endif //__FileManager_H__