#include "ResourceManager.h"
#include "Globals.h"
#include "Application.h"
#include "Mesh.h"
#include "Material.h"
#include "RES_Mesh.h"
#include "RES_Material.h"
#include "Scene.h"
#include "Timer.h"
#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"

#include <map>

ResourceManager::ResourceManager(Application* app, bool start_enabled) : Module(app, "ResourceManager", start_enabled)
{

}
ResourceManager::~ResourceManager()
{}

bool ResourceManager::Start()
{
	LoadMetaFiles();
	GetEngineFiles();

	return true;
}

update_status ResourceManager::Update(float dt)
{

	return UPDATE_CONTINUE;
}

bool ResourceManager::CleanUp()
{
	if (!loadedResources.empty())
	{
		for (std::map<std::string, EngineResource*>::iterator it = loadedResources.begin(); it != loadedResources.end(); ++it)
		{
			LOG("Deleting resource %s",(*it).second->name.c_str());
			(*it).second->Clean();
		}
	}
	loadedResources.clear();
	if (!resourcesMap.empty()) resourcesMap.clear();
	
	return true;
}

void ResourceManager::GetEngineFiles()
{
	//if (!resourcesMap.empty()) resourcesMap.clear();
	std::vector<std::string> ext;
	ext.push_back("psmesh");
	ext.push_back("psmaterial");
	ext.push_back("psscene");
	PathNode engineassets;
	engineassets = App->fileSystem->GetAllFiles("Assets", &ext, nullptr);
	SaveEngineResource(engineassets);
	LOG("Engine resources loaded.");
}

void ResourceManager::LoadMetaFiles()
{
	if (!metaFiles.empty()) metaFiles.clear();
	std::vector<std::string> ext;
	ext.push_back("meta");
	metaNode = App->fileSystem->GetAllFiles("Assets", &ext, nullptr);
	SaveMetaNode(metaNode);
	PathNode tempmetaNode = App->fileSystem->GetAllFiles("DefaultAssets", &ext, nullptr);
	SaveMetaNode(tempmetaNode);
	LOG("Meta files loaded.");
}

void ResourceManager::SaveEngineResource(PathNode n)
{
	if (n.isFile)
	{
		char* buffer = nullptr;		
		uint size = App->fileSystem->Load(n.path.c_str(), &buffer);
		if (size > 0)
		{
			RES_Mesh* mesh = nullptr;
			RES_Material* mat = nullptr;
			Scene* scene = nullptr;
			JSonHandler* node = new JSonHandler(buffer);
			//LOG("File path: %s",n.path.c_str());
			switch ((int)node->GetNum("Type"))
			{
				case MESH_RES:
					mesh = new RES_Mesh();
					mesh->LoadResource(node);
					mesh->SetFullPath(n.localPath);
					resourcesMap.emplace(mesh->UUID,mesh->resource);
					//LOG("Saved mesh res");
					break;
				case MATERIAL_RES:
					mat = new RES_Material();
					mat->LoadResource(node);
					mat->SetFullPath(n.localPath);
					resourcesMap.emplace(mat->UUID,mat->resource);
					//LOG("Saved material res");
					break;
				case SCENE_RES: 				
					scene = new Scene();
					scene->LoadResource(node);
					scene->SetFullPath(n.localPath);
					resourcesMap.emplace(scene->UUID, scene->resource);
					//LOG("Saved scene res");
					break;
			}
			delete node;
			RELEASE_ARRAY(buffer);
		}
	}
	else if (!n.children.empty())
	{
		for (int i = 0; i < n.children.size(); i++)
		{
			SaveEngineResource(n.children[i]);
		}
	}
}

void ResourceManager::SaveMetaNode(PathNode n)
{
	if (n.isFile)
	{
		char* buffer = nullptr;
		MetaFile file;
		uint size = App->fileSystem->Load(n.path.c_str(),&buffer);
		if (size > 0)
		{
			JSonHandler* node = new JSonHandler(buffer);
			file.modTime = node->GetNum("LastModificationTime");
			file.ID = node->GetString("UUID");
			file.name = node->GetString("Name");
			file.filePath = n.path;
			//file.filePath = node->GetString("FilePath");
			//LOG("New meta name: %s",file.name.c_str());
			file.resourceID = node->GetString("ResourceID");
			if (node->LoadArray("Objects"))
			{
				int num = node->GetArrayCount("Objects");
				if (num > 0)
				{
					for (int i = 0; i < num; i++)
					{
						JSonHandler node2 = node->GetNodeArray("Objects", i);
						node2.LoadArray("Resources");
						int num2 = node2.GetArrayCount("Resources");
						if (num2 > 0)
						{
							std::vector<std::string> temp;
							for (int i2 = 0; i2 < num2; i2++)
							{
								temp.push_back(node2.GetStringArray("Resources", i2));
							}
							file.resourcesIDs.push_back(temp);
						}
					}
				}

			}
			metaFiles.emplace(App->fileSystem->GetFileName(n.localPath.c_str()),file);
			delete node;
			RELEASE_ARRAY(buffer);
		}
	}
	else if (!n.children.empty())
	{
		for (int i = 0; i < n.children.size(); i++)
		{
			SaveMetaNode(n.children[i]);
		}
	}
}

bool ResourceManager::LoadMeshResource(RES_Mesh* mesh)
{
	bool ret = false;
	char* buffer = nullptr;
	uint size = App->fileSystem->Load(mesh->libPath.c_str(), &buffer); //Load mesh buffer file
	if (size > 0)
	{
		App->fileSystem->LoadMesh(mesh, buffer); //Load mesh
		ret = GenerateMeshBuffer(mesh);
		if(ret) loadedResources.emplace(mesh->UUID, mesh->resource);
				
		RELEASE_ARRAY(buffer);
		return ret;
	}
	return ret;
}

bool ResourceManager::LoadMaterialResource(RES_Material* mat)
{
	bool ret = false;
	App->fileSystem->LoadMaterial(mat/*, &buffer,size*/); //Load material
	ret = GenerateMaterialBuffer(mat);
	if(ret) loadedResources.emplace(mat->UUID, mat->resource);
	return ret;
}

bool ResourceManager::LoadSceneResource(Scene* scene)
{
	bool ret = true;
	char* buffer = nullptr;
	std::string name = scene->name.append(".psscene");
	if (metaFiles.find(name) != metaFiles.end())
	{
		std::string path = App->fileSystem->GetPathAndFile(metaFiles[name].filePath.c_str());
		uint size = App->fileSystem->Load(path.c_str(), &buffer);
		if (size > 0)
		{
			JSonHandler* node = new JSonHandler(buffer);
			ret = scene->LoadScene(node);
			if (ret) loadedResources.emplace(scene->UUID, scene->resource);
		}
		else
		{
			LOG("Buffer size 0 or less");
			ret = false;
		}
	}
	else
	{	
		LOG("Scene meta not found");
		ret = false;
	}
	return ret;
}

bool ResourceManager::LoadResource(std::string uuid)
{
	bool ret = false;
	if (loadedResources.find(uuid) != loadedResources.end()) ret = true;//Resource already loaded
	else //Resource not loaded
	{
		if (resourcesMap.find(uuid) != resourcesMap.end())
		{
			switch (resourcesMap[uuid]->type)
			{
			case MESH_RES:
				ret = LoadMeshResource(resourcesMap[uuid]->AsMesh());
				loadedResources[uuid]->clean = false;
				break;
			case MATERIAL_RES:
				ret = LoadMaterialResource(resourcesMap[uuid]->AsMaterial());
				loadedResources[uuid]->clean = false;
				break;
			case SCENE_RES:
				ret = LoadSceneResource(resourcesMap[uuid]->AsScene());
				break;
			default:
				ret = false;
				LOG("Resource type error.");
				break;
			}
			
		}
		else
		{
			ret = false;
			LOG("Resource not found.");
		}
	}
	return ret;
}

void ResourceManager::DeleteResource(std::string uuid)
{
	if (loadedResources.find(uuid) != loadedResources.end())
	{
		loadedResources[uuid]->Clean();
		loadedResources.erase(uuid);
	}

	if (resourcesMap.find(uuid) != resourcesMap.end())
	{
		if (metaFiles.find(resourcesMap[uuid]->name) != metaFiles.end())
		{
			App->fileSystem->Remove(metaFiles[resourcesMap[uuid]->name].filePath.c_str());
			metaFiles.erase(resourcesMap[uuid]->name);
		}
		App->fileSystem->Remove(resourcesMap[uuid]->libPath.c_str());
		std::string resPath = App->fileSystem->SearchFile(resourcesMap[uuid]->name.c_str());
		if(resPath.compare("") != 0) App->fileSystem->Remove(resPath.c_str());
		resourcesMap.erase(uuid);
	}
}

void ResourceManager::DeleteResourceByPath(std::string path)
{
	std::string uuid;
	std::string name = App->fileSystem->GetFileAndExtension(path.c_str());

	if (metaFiles.find(name) != metaFiles.end())
	{
		uuid = metaFiles[name].resourceID;
		App->fileSystem->Remove(metaFiles[name].filePath.c_str());
		metaFiles.erase(name);

		if (loadedResources.find(uuid) != loadedResources.end())
		{
			loadedResources[uuid]->Clean();
			loadedResources.erase(uuid);
		}

		if (resourcesMap.find(uuid) != resourcesMap.end())
		{
			App->fileSystem->Remove(resourcesMap[uuid]->libPath.c_str());
			App->fileSystem->Remove(path.c_str());
			resourcesMap.erase(uuid);
		}
	}
}


bool ResourceManager::CheckMetaFile(std::string name)
{
	if (metaFiles.find(name) != metaFiles.end())
	{
		//LOG("Meta file found");
		return true;
	}
	else return false;
}

bool ResourceManager::CheckMetaPath(std::string path,std::string name)
{
	if (metaFiles.find(name) != metaFiles.end())
	{
		std::string metaLoc = path;
		metaLoc.append(".meta");
		//LOG("Meta path: %s",metaFiles[name].filePath);
		if (App->fileSystem->Exists(metaLoc.c_str())) return true;
		else
		{		
			//File moved or renamed
			if (App->fileSystem->Exists(metaFiles[name].filePath.c_str()))
			{				
				//Meta file found, move it
				App->fileSystem->MoveFileTo(metaFiles[name].filePath.c_str(), metaLoc.c_str());
				metaFiles[name].filePath = metaLoc;
				resourcesMap[metaFiles[name].resourceID]->currentPath = path;
				LOG("Meta file %s moved",metaLoc.c_str());
				return true;
			}	
			else
			{
				//Old meta file not found
				return false;
			}
		}
	}
	return false;
}

GameObject* ResourceManager::ImportFBXFromMeta(std::string id)
{
	GameObject* go = nullptr;
	if (metaFiles.find(id) != metaFiles.end())
	{
		//LOG("Meta file loaded");
		MetaFile meta = metaFiles[id];
		go = new GameObject(meta.name.c_str());

		if (!meta.resourcesIDs.empty())
		{
			for (std::vector<std::vector<std::string> >::const_iterator it = meta.resourcesIDs.begin(); it != meta.resourcesIDs.end(); ++it)
			{
				if (!(*it).empty())
				{
					GameObject* child = go->CreateChild();
					std::vector<std::string> tempV = (*it);		
					for (int i = 0; i < tempV.size(); i++)
					{
						if (LoadResource(tempV[i]))
						{
							EngineResource* res = GetResource(tempV[i]);
							if (res != nullptr)
							{
								Component* tempComp = nullptr;
								Mesh* mesh = nullptr;
								Material* mat = nullptr;
								switch (res->type)
								{
								case MESH_RES:
									tempComp = child->AddComponent(MESH_COMP);
									if (tempComp != nullptr)
									{
										mesh = tempComp->AsMesh();
										if (mesh != nullptr)
										{
											mesh->SetMesh(GetResource(tempV[i])->AsMesh());
										}
									}
									break;
								case MATERIAL_RES:
									tempComp = child->AddComponent(MATERIAL_COMP);
									if (tempComp != nullptr)
									{
										mat = tempComp->AsMaterial();
										if (mat != nullptr)
										{
											mat->SetMaterial(GetResource(tempV[i])->AsMaterial());
											if (mat->gameobject->GetFirstComponentType(MESH_COMP) != nullptr)
											{
												if (mat->gameobject->GetFirstComponentType(MESH_COMP)->AsMesh() != nullptr)
												{
													mat->gameobject->GetFirstComponentType(MESH_COMP)->AsMesh()->SetMaterial(mat->GetMaterial());
												}
											}
										}
									}
									break;
								}
							}
						}
					}
				}				
			}
		}
	}
	return go;
}

void ResourceManager::ImportFBXFromMeta(std::string id, GameObject* go)
{
	if (metaFiles.find(id) != metaFiles.end())
	{
		MetaFile meta = metaFiles[id];
		go->name = meta.name.c_str();
		//LOG("Meta file loaded");
		if (!meta.resourcesIDs.empty())
		{
			//LOG("Objects size %d", meta.resourcesIDs.size());
			for (std::vector<std::vector<std::string> >::const_iterator it = meta.resourcesIDs.begin(); it != meta.resourcesIDs.end(); ++it)
			{
				if (!(*it).empty())
				{
					GameObject* child = go->CreateChild();
					std::vector<std::string> tempV = (*it);
					//LOG("Resources size %d", tempV.size());
					for(int i = 0;i < tempV.size();i++)
					{
						//LOG("Resource %s",tempV[i].c_str());
						if (LoadResource(tempV[i]))
						{
							EngineResource* res = GetResource(tempV[i]);
							if (res != nullptr)
							{
								Component* tempComp = nullptr;
								Mesh* mesh = nullptr;
								Material* mat = nullptr;
								switch (res->type)
								{
								case MESH_RES:
									tempComp = child->AddComponent(MESH_COMP);
									if (tempComp != nullptr)
									{
										mesh = tempComp->AsMesh();
										if (mesh != nullptr)
										{
											mesh->SetMesh(GetResource(tempV[i])->AsMesh());
										}
									}
									break;
								case MATERIAL_RES:
									tempComp = child->AddComponent(MATERIAL_COMP);
									if (tempComp != nullptr)
									{
										mat = tempComp->AsMaterial();
										if (mat != nullptr)
										{
											mat->SetMaterial(GetResource(tempV[i])->AsMaterial());
											if (mat->gameobject->GetFirstComponentType(MESH_COMP) != nullptr)
											{
												if (mat->gameobject->GetFirstComponentType(MESH_COMP)->AsMesh() != nullptr)
												{
													mat->gameobject->GetFirstComponentType(MESH_COMP)->AsMesh()->SetMaterial(mat->GetMaterial());
												}
											}
										}
									}
									break;
								}
							}
						}
					}
				}
			}
		}
	}
}

EngineResource* ResourceManager::GetMetaResource(std::string id)
{
	if (metaFiles.find(id) != metaFiles.end())
	{
		if (metaFiles[id].resourceID.compare("") != 0)
		{
			return GetResource(metaFiles[id].resourceID);
		}
	}
	
	return nullptr;
}

void ResourceManager::FreeResource(std::string uuid)
{
	if (loadedResources.find(uuid) != loadedResources.end())
	{
		loadedResources[uuid]->references--;
		if (loadedResources[uuid]->references <= 0)
		{
			loadedResources[uuid]->Clean();
			loadedResources.erase(uuid);
		}
	}
}

GameObject* ResourceManager::ImportFBX(const char* path)
{
	if (App->scene->state == SCENE_STOP)
	{
		GameObject* go = App->scene->GetActiveScene()->CreateEmptyGameobject();
		App->fileSystem->ImportFBX(path,go);
		return go;
	}
}

void ResourceManager::ImportFBX(const char* path, GameObject* go)
{
	if (App->scene->state == SCENE_STOP) App->fileSystem->ImportFBX(path, go);
}

RES_Material* ResourceManager::ImportTexture(const char* path)
{
	if (App->scene->state == SCENE_STOP)
	{
		RES_Material* mat = new RES_Material();
		App->fileSystem->GetDroppedFile(path,nullptr,mat);
		return mat;
	}
}

bool ResourceManager::GenerateMeshBuffer(RES_Mesh* mesh)
{
	if (mesh->verticesSize > 0)
	{
		glGenVertexArrays(1, &mesh->VAO);
		glBindVertexArray(mesh->VAO);

		//VertexBuffer
		glGenBuffers(1, (GLuint*)&(mesh->idVertex));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->idVertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->verticesSize * 3, mesh->verticesArray, GL_STATIC_DRAW);

		//Index buffer
		if (mesh->indexSize > 0)
		{
			glGenBuffers(1, &(mesh->idIndex));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->idIndex);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->indexSize, mesh->indicesArray, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}

		//Textures buffer
		if (mesh->textSize > 0)
		{
			glGenBuffers(1, &mesh->idText);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->idText);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->textSize * 2, mesh->texturesArray, GL_STATIC_DRAW);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
		}

		//Normals buffer
		if (mesh->normalsSize > 0)
		{
			glGenBuffers(1, (GLuint*)&mesh->idNormals);
			glBindBuffer(GL_NORMAL_ARRAY, mesh->idNormals);
			glBufferData(GL_NORMAL_ARRAY, sizeof(float) * mesh->verticesSize * 3, mesh->normalsArray, GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(2);
		}
		glBindVertexArray(0);
		LOG("Mesh buffer generated");
		return true;
	}
	return false;
}

bool ResourceManager::GenerateMaterialBuffer(RES_Material* material)
{
	if (material != nullptr)
	{
		if (material->textureID != -1)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &material->textureID);
			glBindTexture(GL_TEXTURE_2D, material->textureID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, material->textWidth, material->textHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, material->textData);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
			LOG("Texture buffer generated");
			return true;
		}
	}
	return false;
}

std::string ResourceManager::SaveResource(std::string uuid)
{
	JSonHandler file;
	resourcesMap[uuid]->SaveResource(&file);

	//Write to file
	char* buffer = nullptr;
	uint size = file.Serialize(&buffer);
	App->fileSystem->Save(resourcesMap[uuid]->currentPath.c_str(), buffer, size);
	RELEASE_ARRAY(buffer);

	return resourcesMap[uuid]->currentPath.c_str();
}

void ResourceManager::SaveResource(std::string uuid, std::string path)
{
	JSonHandler file;
	resourcesMap[uuid]->SaveResource(&file);

	//Remove old files
	if(App->fileSystem->Exists(resourcesMap[uuid]->currentPath.c_str())) App->fileSystem->Remove(resourcesMap[uuid]->currentPath.c_str());
	std::string temp = resourcesMap[uuid]->name;/* .append(resourcesMap[uuid]->extension);*/
	if (CheckMetaFile(temp)) App->fileSystem->Remove(temp.c_str());	
	
	//Write to file
	char* buffer = nullptr;
	uint size = file.Serialize(&buffer);	
	resourcesMap[uuid]->SetAssetsPath(path);
	//LOG("Asset path: %s",resourcesMap[uuid]->currentPath.c_str());
	App->fileSystem->Save(resourcesMap[uuid]->currentPath.c_str(), buffer, size);
	CreateResourceMeta(uuid);
	RELEASE_ARRAY(buffer);
}

void ResourceManager::PlaceResource(EngineResource* res)
{
	resourcesMap.emplace(res->UUID, res);
}

EngineResource* ResourceManager::GetResource(std::string uuid)
{
	if (loadedResources.find(uuid) != loadedResources.end()) return loadedResources[uuid];//Resource loaded
	else //Resource not loaded
	{
		if (LoadResource(uuid))
		{
			return loadedResources[uuid];
		}
		else
		{
			LOG("Resource not loaded.");
			return nullptr;
		}
	}
}

void ResourceManager::CreateResourceMeta(std::string uuid)
{
	JSonHandler file;

	file.SaveNum("LastModificationTime", App->fileSystem->GetLastModTime(resourcesMap[uuid]->currentPath.c_str()));
	file.SaveString("UUID", App->GenerateUUID_V4().c_str());
	file.SaveString("ResourceID", uuid.c_str());

	//Write to file
	char* buffer = nullptr;
	uint size = file.Serialize(&buffer);
	std::string fileName = resourcesMap[uuid]->currentPath;
	fileName.append(".meta");
	App->fileSystem->Save(fileName.c_str(), buffer, size);
	RELEASE_ARRAY(buffer);
	App->resourceManager->LoadMetaFiles();
}

EngineResource* ResourceManager::GetResourceByName(std::string name)
{
	for (std::map<std::string, EngineResource*>::iterator it = resourcesMap.begin(); it != resourcesMap.end(); ++it)
	{
		if ((*it).second->name.append((*it).second->extension).compare(name) == 0)
		{
			return (*it).second;
		}
	}
	return nullptr;
}

bool ResourceManager::CreateResourceMetaByName(std::string name)
{
	EngineResource* temp = GetResourceByName(name);
	if (temp != nullptr) CreateResourceMeta(temp->UUID);
	else return false;

	return true;
}