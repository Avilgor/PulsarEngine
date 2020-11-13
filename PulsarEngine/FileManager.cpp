#include "FileManager.h"
#include "Globals.h"
#include "Application.h"
#include "RES_Mesh.h"
#include "RES_Material.h"
#include "Scene.h"

FileManager::FileManager(Application* app, bool start_enabled = true) : Module(app, "FileManager", start_enabled)
{

}
FileManager::~FileManager()
{}

bool FileManager::Start()
{
	return true;
}

update_status FileManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool FileManager::CleanUp()
{

	return true;
}

void FileManager::SaveMeshResource(RES_Mesh* mesh)
{
	meshResources.emplace(mesh->UUID,mesh);
}

void FileManager::SaveMaterialResource(RES_Material* mat)
{
	materialResources.emplace(mat->UUID, mat);
}

void FileManager::SaveSceneResource(Scene* scene)
{
	sceneResources.emplace(scene->UUID, scene);
}

RES_Mesh* FileManager::GetMeshResource(std::string uuid)
{
	if (meshResources.find(uuid) != meshResources.end()) return meshResources[uuid];
	return nullptr;
}

RES_Material* FileManager::GetMaterialResource(std::string uuid)
{
	if (materialResources.find(uuid) != materialResources.end()) return materialResources[uuid];
	return nullptr;
}

Scene* FileManager::GetSceneResource(std::string uuid)
{
	if (sceneResources.find(uuid) != sceneResources.end()) return sceneResources[uuid];
	return nullptr;
}