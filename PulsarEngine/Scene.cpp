#include "Globals.h"
#include "Application.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "RES_Mesh.h"
#include "Material.h"
#include "RES_Material.h"
#include "JSonHandler.h"

Scene::Scene()
{
	root = new GameObject("Root");
	//Camera gameobject
}

Scene::Scene(const char* n)
{
	name = n;
	root = new GameObject("Root");
	root->SetUUID("0");
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

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));


	GameObject* trump = new GameObject("Trump", float3(3.0f, 0.0f, 0.0f), float3(-90.0f, 0.0f, 0.0f), float3(0.02f, 0.02f, 0.02f));
	trump->AddComponent(MESH_COMP);
	trump->AddComponent(MATERIAL_COMP);
	root->AddChild(trump);


	GameObject* go2 = new GameObject("Baker House", float3(-1.0f, 0.0f, 0.0f), float3::zero, float3::one);
	go2->AddComponent(MESH_COMP);
	go2->AddComponent(MATERIAL_COMP);
	root->AddChild(go2);


	GameObject* go3 = new GameObject("Chiken", float3(-4.0f, 0.0f, 0.0f), float3::zero, float3(0.01f, 0.01f, 0.01f));
	go3->AddComponent(MESH_COMP);
	go3->AddComponent(MATERIAL_COMP);
	root->AddChild(go3);


	Component* comp = trump->GetFirstComponentType(MESH_COMP);
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
	}

}

update_status Scene::UpdateScene()
{
	update_status ret = UPDATE_CONTINUE;

	App->renderer3D->RenderGroundGrid(10);

	if (root != nullptr)
	{
		root->UpdateTransform();
		root->UpdateGameObject();
		root->DrawMesh();
	}

	return ret;
}

void Scene::CleanScene()
{
	App->editor->EmptySelected();
	root->DeleteAllChilds();
	root->Delete();
	root = nullptr;
	delete this;
}


void Scene::SaveScene()
{
	JSonHandler* saveFile = new JSonHandler();
	
	//Save gameobjects hierarchy
	JSonHandler settings;
	std::string label = "Gameobjets";
	settings.CreateArray(label.c_str());
	root->SaveGameobject(&settings,label.c_str());

	//Save gameobjects stats and components


	//Write to file
	char* buffer = nullptr;
	uint size = settings.Serialize(&buffer);
	std::string fileName = SCENES_PATH;
	fileName.append(name);
	fileName.append(".JSON");
	App->fileSystem->Save(fileName.c_str(), buffer, size);
	RELEASE_ARRAY(buffer);

 	LOG("Scene %s saved!", name.c_str());
}

void Scene::LoadScene()
{

	LOG("Scene %s loaded!", name.c_str());
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
	go->AddComponent(MESH_COMP);
	root->AddChild(go);
	Component* comp = go->GetFirstComponentType(MESH_COMP);
	if (comp != nullptr)
	{
		if (comp->AsMesh() != nullptr) comp->AsMesh()->CreateCube();
	}
}

void Scene::CreatePyramid()
{
	GameObject* go = new GameObject("Pyramid");
	go->AddComponent(MESH_COMP);
	root->AddChild(go);
	Component* comp = go->GetFirstComponentType(MESH_COMP);
	if (comp != nullptr)
	{
		if (comp->AsMesh() != nullptr) comp->AsMesh()->CreatePyramid();
	}
}

void Scene::CreatePlane()
{
	GameObject* go = new GameObject("Plane");
	go->AddComponent(MESH_COMP);
	root->AddChild(go);
	Component* comp = go->GetFirstComponentType(MESH_COMP);
	if (comp != nullptr)
	{
		if (comp->AsMesh() != nullptr) comp->AsMesh()->CreatePlane(5);
	}
}