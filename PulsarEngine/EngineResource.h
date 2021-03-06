#ifndef __EngineResource_H__
#define __EngineResource_H__

#include "Globals.h"

#include <string>

class JSonHandler;
class RES_Mesh;
class RES_Material;
class Scene;

class EngineResource
{
public:

	EngineResource(RESOURCE_TYPE t);
	EngineResource(RESOURCE_TYPE t,std::string uuid);
	~EngineResource();
	virtual void Clean() {}
	virtual void SaveResource(JSonHandler* file) {}
	virtual void LoadResource(JSonHandler* file) {}
	void SetAssetsPath(std::string path);
	void SetFullPath(std::string path);
	void UpdatePath();

	RES_Mesh* AsMesh() { return meshRes; }
	RES_Material* AsMaterial() { return matRes; }
	Scene* AsScene() { return sceneRes; }

public:
	std::string UUID = " ";
	std::string name = "New Resource";
	std::string extension = " ";
	std::string libPath = " ";
	std::string assetPath = " ";
	std::string currentPath = " ";
	RESOURCE_TYPE type;
	bool clean;
	bool loaded = false;
	int references = 0;
	EngineResource* resource = nullptr;

private:
	void SetExtension();

protected:
	RES_Mesh* meshRes = nullptr;
	RES_Material* matRes = nullptr;
	Scene* sceneRes = nullptr;
};

#endif //__EngineResource_H__