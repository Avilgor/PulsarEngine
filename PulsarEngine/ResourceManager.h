#ifndef __FileManager_H__
#define __FileManager_H__

#include "Module.h"
#include "PathNode.h"
//#include "Timer.h"

#include <string>
#include <map>

class RES_Material;
class RES_Mesh;
class Scene;
class EngineResource;
class GameObject;


struct MetaFile
{
	uint64 modTime;
	std::string ID = "";
	std::string name = "";
	std::vector<std::vector<std::string> > resourcesIDs;
	std::string resourceID = "";
	std::string filePath = "";
};


class ResourceManager : public Module
{
public:
	ResourceManager(Application* app, bool start_enabled = true);
	~ResourceManager();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	void LoadMetaFiles();
	
	GameObject* ImportFBX(const char* path);
	void ImportFBX(const char* path, GameObject* go);
	RES_Material* ImportTexture(const char* path);
	GameObject* ImportFBXFromMeta(std::string id);
	void ImportFBXFromMeta(std::string id,GameObject* go);
	EngineResource* GetResource(std::string uuid);
	EngineResource* GetMetaResource(std::string id);

	bool CheckMetaFile(std::string name);
	bool CheckMetaPath(std::string path,std::string name);
	//std::string Save_RES_Mesh(RES_Mesh* mesh,const char* path);
	//std::string Save_RES_Material(RES_Material* mat, const char* path);
	std::string SaveResource(std::string uuid);
	void SaveResource(std::string uuid,std::string path);
	void PlaceResource(EngineResource* res);	
	void CreateResourceMeta(std::string uuid);
	EngineResource* GetResourceByName(std::string name);
	bool CreateResourceMetaByName(std::string name);
	void DeleteResource(std::string uuid);
	void DeleteResourceByPath(std::string path);

	bool LoadResource(std::string uuid);
	void FreeResource(std::string uuid);

private:
	void GetEngineFiles();
	void SaveMetaNode(PathNode node);
	void SaveEngineResource(PathNode node);
	bool LoadMeshResource(RES_Mesh* mesh);
	bool LoadMaterialResource(RES_Material* mat);
	bool LoadSceneResource(Scene* scene);
	bool GenerateMeshBuffer(RES_Mesh* mesh);
	bool GenerateMaterialBuffer(RES_Material* mat);

private:
	
	std::map<std::string, EngineResource*> resourcesMap;
	std::map<std::string, EngineResource*> loadedResources;
	std::map<std::string, MetaFile> metaFiles;
	PathNode metaNode;

};

#endif //__FileManager_H__