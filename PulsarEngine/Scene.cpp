#include "Globals.h"
#include "Application.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "JSonHandler.h"
#include "ResourceManager.h"

#include <map>

Scene::Scene() : EngineResource(SCENE_RES)
{
	root = new GameObject("Root");
	root->SetUUID("0");
	sceneRes = this;
}

Scene::Scene(const char* n) : EngineResource(SCENE_RES)
{
	name = n;
	root = new GameObject("Root");
	root->SetUUID("0");
	sceneRes = this;
}

Scene::Scene(std::string id) : EngineResource(SCENE_RES,id)
{
	root = new GameObject("Root");
	root->SetUUID("0");
	sceneRes = this;
}

Scene::Scene(const char* n, std::string id) : EngineResource(SCENE_RES, id)
{
	name = n;
	root = new GameObject("Root");
	root->SetUUID("0");
	sceneRes = this;
}

Scene::~Scene()
{
	if (root != nullptr)
	{
		root->DeleteAllChilds();
		root->Delete();
	}
}

void Scene::StartScene()
{	
	LOG("Starting scene %s",name.c_str());

	//App->camera->Move(vec(0.0f, 10.0f, 5.0f));
	//App->camera->Look(float3(0, 0, 0));
	
	GameObject* trump = new GameObject("Trump", float3(3.0f, 0.0f, 0.0f), float3(-90.0f, 0.0f, 0.0f), float3(0.02f, 0.02f, 0.02f));
	root->AddChild(trump);
	App->resourceManager->ImportFBX("Assets/3D/Trump/trump.fbx",trump);

	/*GameObject* go2 = new GameObject("Baker House", float3(-1.0f, 0.0f, 0.0f), float3::zero, float3::one);
	root->AddChild(go2);
	App->resourceManager->ImportFBX("Assets/3D/Baker/BakerHouse.fbx",go2);


	GameObject* go3 = new GameObject("Chiken", float3(-4.0f, 0.0f, 0.0f), float3::zero, float3(0.01f, 0.01f, 0.01f));
	root->AddChild(go3);
	App->resourceManager->ImportFBX("Assets/3D/Chiken/cock.fbx",go3);*/

	/*Component* comp = trump->GetFirstComponentType(MESH_COMP);
	if (comp != nullptr)
	{
		if (comp->AsMesh() != nullptr) App->fileSystem->ImportMesh(comp->AsMesh(), "Assets/3D/Trump/trump.FBX");
	}

	comp = trump->GetFirstComponentType(MATERIAL_COMP);
	if (comp != nullptr)
	{
		if (comp->AsMaterial() != nullptr)
		{
			comp->AsMaterial()->LoadTextureNewMaterial("Assets/3D/Trump/tumpLPcolors.png");
			trump->GetFirstComponentType(MESH_COMP)->AsMesh()->SetAllMeshesMaterial(comp->AsMaterial()->GetMaterial(0));
		}
	}

	Component* comp2 = go2->GetFirstComponentType(MESH_COMP);
	if (comp2 != nullptr)
	{
		if (comp2->AsMesh() != nullptr) App->fileSystem->ImportMesh(comp2->AsMesh(), "Assets/3D/Baker/BakerHouse.fbx");
	}

	comp2 = go2->GetFirstComponentType(MATERIAL_COMP);
	if (comp2 != nullptr)
	{
		if (comp2->AsMaterial() != nullptr)
		{
			comp2->AsMaterial()->LoadTextureNewMaterial("Assets/3D/Baker/Baker_house.png");
			go2->GetFirstComponentType(MESH_COMP)->AsMesh()->SetAllMeshesMaterial(comp2->AsMaterial()->GetMaterial(0));
		}
	}

	Component* comp3 = go3->GetFirstComponentType(MESH_COMP);
	if (comp3 != nullptr)
	{
		if (comp3->AsMesh() != nullptr) App->fileSystem->ImportMesh(comp3->AsMesh(), "Assets/3D/Chiken/cock.fbx");
	}

	comp3 = go3->GetFirstComponentType(MATERIAL_COMP);
	if (comp3 != nullptr)
	{
		comp3 = comp3->AsMaterial();
		if (comp3 != nullptr)
		{
			Mesh* meshComp = go3->GetFirstComponentType(MESH_COMP)->AsMesh();
			RES_Material* tempMat = nullptr;
			comp3->AsMaterial()->LoadTextureNewMaterial("Assets/3D/Chiken/textures/rooster_color.png");
			tempMat = comp3->AsMaterial()->GetLastMaterial();
			if (tempMat != nullptr) meshComp->SetMeshMaterial(tempMat, 0);
		}
	}*/
}

update_status Scene::UpdateScene(float dt)
{
	update_status ret = UPDATE_CONTINUE;
	//LOG("Scene update");
	App->renderer3D->RenderGroundGrid(10);

	if (root != nullptr)
	{
		root->UpdateTransform();
		//Temp
		/*if(App->scene->state == SCENE_RUNNING)*/ root->UpdateGameObject();

		root->DrawMesh();
	}

	return ret;
}

update_status Scene::PostUpdateScene(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	return ret;
}

void Scene::Clean()
{
	App->editor->EmptySelected();
	root->DeleteAllChilds();
	root->Delete();
	root = nullptr;
	delete this;
}


void Scene::SaveResource(JSonHandler* file)
{	
	//Save gameobjects 
	file->SaveString("UUID", UUID.c_str());
	file->SaveNum("Type", (double)type);
	file->SaveString("Name",name.c_str());
	std::string label = "Gameobjects";
	file->CreateArray(label.c_str());
	root->SaveGameobject(file,label.c_str());

 	LOG("Scene %s saved!", name.c_str());
}

void Scene::LoadResource(JSonHandler* file)
{
	std::map <std::string, GameObject*> gameobjectsLoaded;
	UUID = file->GetString("UUID");
	name = file->GetString("Name");
	file->LoadArray("Gameobjects");
	int num = file->GetArrayCount("Gameobjects");
	gameobjectsLoaded.emplace(root->UUID,root);

	if (num > 1)//Start with 1 to avoid root
	{
		//Load all gameobjects
		for (int a = 1; a < num; a++)
		{
			GameObject* go = new GameObject();
			JSonHandler temp = file->GetNodeArray("Gameobjects",a);
			go->LoadGameObject(&temp);
			gameobjectsLoaded.emplace(go->UUID, go);
		}
		LOG("Scene gameobjects loaded...");
		
		//Set gameobjects hierarchy
		for (std::map<std::string,GameObject*>::iterator it = gameobjectsLoaded.begin(); it != gameobjectsLoaded.end(); it++)
		{
			if ((*it).second->UUID.compare("0") != 0)
			{
				gameobjectsLoaded[(*it).second->parentUUID]->AddChild((*it).second);
				(*it).second->SetParent(gameobjectsLoaded[(*it).second->parentUUID]);
			}
		}
		LOG("Gameobjects childs set");
	}
	gameobjectsLoaded.clear();
	LOG("Scene %s loaded!", name.c_str());
}

void Scene::SaveTempScene()
{
	//Save gameobjects hierarchy
	JSonHandler settings;
	settings.SaveString("Name", name.c_str());
	std::string label = "Gameobjects";
	settings.CreateArray(label.c_str());
	root->SaveGameobject(&settings, label.c_str());

	//Write to file
	char* buffer = nullptr;
	uint size = settings.Serialize(&buffer);
	std::string fileName = SCENES_PATH;
	fileName.append(name);
	fileName.append("temp.psscene");
	App->fileSystem->Save(fileName.c_str(), buffer, size);
	RELEASE_ARRAY(buffer);

	LOG("Recover scene created", name.c_str());
}

void Scene::LoadTempScene()
{
	App->editor->EmptySelected();
	root->DeleteAllChilds();
	char* buffer = nullptr;
	std::string path = SCENES_PATH;
	path.append(name.c_str());
	path.append("temp.JSON");
	uint size = App->fileSystem->Load(path.c_str(), &buffer);
	if (size > 0)
	{
		JSonHandler* node = new JSonHandler(buffer);		
		LoadResource(node);

		delete node;
		RELEASE_ARRAY(buffer);
	}
	else LOG("Error recovering scene.");
}

GameObject* Scene::CreateEmptyGameobject()
{
	GameObject* go = new GameObject("NewGameobject");
	root->AddChild(go);
	return go;
}

GameObject* Scene::CreateEmptyGameobject(const char* name)
{
	GameObject* go = new GameObject(name);
	root->AddChild(go);
	return go;
}

void Scene::CreateCube()
{
	GameObject* go = new GameObject("Cube");
	root->AddChild(go);
	App->resourceManager->ImportFBX("DefaultAssets/Models/cube.fbx", go);
}

void Scene::CreatePyramid()
{
	GameObject* go = new GameObject("Pyramid");
	root->AddChild(go);
	App->resourceManager->ImportFBX("DefaultAssets/Models/cone.fbx", go);
}

void Scene::CreatePlane()
{
	GameObject* go = new GameObject("Plane");
	root->AddChild(go);
	App->resourceManager->ImportFBX("DefaultAssets/Models/plane.fbx", go);
}

void Scene::CreateSphere()
{
	GameObject* go = new GameObject("Sphere");
	root->AddChild(go);
	App->resourceManager->ImportFBX("DefaultAssets/Models/sphere.fbx", go);
}

void Scene::CreateCylinder()
{
	GameObject* go = new GameObject("Cylinder");
	root->AddChild(go);
	App->resourceManager->ImportFBX("DefaultAssets/Models/cylinder.fbx", go);
}

void Scene::CreateTorus()
{
	GameObject* go = new GameObject("Torus");
	root->AddChild(go);
	App->resourceManager->ImportFBX("DefaultAssets/Models/torus.fbx", go);
}