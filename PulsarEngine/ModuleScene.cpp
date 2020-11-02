#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "RES_Material.h"

struct MaterialInfo;


ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app,"Scene",start_enabled)
{}

ModuleScene::~ModuleScene()
{}

bool ModuleScene::Init()
{	
	return true;
}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;	

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	root = new GameObject("Root");
	
	/*GameObject* trump = new GameObject("Trump",float3(3.0f,0.0f,0.0f),float3(-90.0f,0.0f,0.0f),float3(0.02f,0.02f,0.02f));
	trump->AddComponent(MESH_COMP);
	trump->AddComponent(MATERIAL_COMP);
	root->AddChild(trump);
	*/
	/*
	GameObject* go2 = new GameObject("Baker House",float3(-1.0f,0.0f,0.0f),float3::zero,float3::one);
	go2->AddComponent(MESH_COMP);
	go2->AddComponent(MATERIAL_COMP);
	root->AddChild(go2);
	*/

	GameObject* go3 = new GameObject("Chiken", float3(-4.0f, 0.0f, 0.0f), float3::zero, float3(0.01f,0.01f,0.01f));
	go3->AddComponent(MESH_COMP);
	go3->AddComponent(MATERIAL_COMP);
	root->AddChild(go3);


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
	}*/
	/*
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
	*/
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
			if (tempMat != nullptr) meshComp->SetMeshMaterial(tempMat,0);
				
		}
	}

	return ret;
}


bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro scene");
	App->editor->EmptySelected();
	root->DeleteAllChilds();	
	return true;
}

GameObject* ModuleScene::CreateEmptyGameobject()
{
	GameObject* go = new GameObject("NewGameobject");
	root->AddChild(go);
	return go;
}

GameObject* ModuleScene::CreateEmptyGameobject(const char* name)
{
	GameObject* go = new GameObject(name);
	root->AddChild(go);
	return go;
}

// PreUpdate
update_status ModuleScene::PreUpdate(float dt)
{	
	return UPDATE_CONTINUE;
}

// Update
update_status ModuleScene::Update(float dt)
{
    App->renderer3D->RenderGroundGrid(10);

	if (root != nullptr)
	{
		root->UpdateTransform();
		root->UpdateGameObject();
		root->DrawMesh();
	}

	return UPDATE_CONTINUE;
}

// PostUpdate
update_status ModuleScene::PostUpdate(float dt)
{	
		
	return UPDATE_CONTINUE;
}

void ModuleScene::CreateCube()
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

void ModuleScene::CreatePyramid()
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

void ModuleScene::CreatePlane()
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
