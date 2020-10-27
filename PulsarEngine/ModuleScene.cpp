#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"


ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
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
	
	GameObject* trump = new GameObject("Trump",float3(3.0f,0.0f,0.0f),float3(-90.0f,0.0f,0.0f),float3(0.02f,0.02f,0.02f));
	trump->AddComponent(MESH_COMP);
	trump->AddComponent(MATERIAL_COMP);
	//root->AddChild(warrior);


	GameObject* go2 = new GameObject("Baker House",float3(-1.0f,0.0f,0.0f),float3::zero,float3::one);
	go2->AddComponent(MESH_COMP);
	go2->AddComponent(MATERIAL_COMP);
	root->AddChild(go2);
	go2->AddChild(trump);

	Component* comp = trump->GetFirstComponentType(MESH_COMP);
	if (comp != nullptr)
	{
		if (comp->AsMesh() != nullptr) App->fbxLoader->ImportMesh(comp->AsMesh(), "Assets/3D/Trump/trump.FBX");
	}

	comp = trump->GetFirstComponentType(MATERIAL_COMP);
	if (comp != nullptr)
	{
		if (comp->AsMaterial() != nullptr)
		{
			comp->AsMaterial()->LoadTextureMaterial("Assets/3D/Trump/tumpLPcolors.png");
			trump->GetFirstComponentType(MESH_COMP)->AsMesh()->SetAllMeshesMaterial(comp->AsMaterial()->GetMaterial(0));
		}
	}

	Component* comp2 = go2->GetFirstComponentType(MESH_COMP);
	if (comp2 != nullptr)
	{
		if (comp2->AsMesh() != nullptr) App->fbxLoader->ImportMesh(comp2->AsMesh(), "Assets/3D/Baker/BakerHouse.fbx");
	}

	comp2 = go2->GetFirstComponentType(MATERIAL_COMP);
	if (comp2 != nullptr)
	{
		if (comp2->AsMaterial() != nullptr)
		{
			comp2->AsMaterial()->LoadTextureMaterial("Assets/3D/Baker/Baker_house.png");
			go2->GetFirstComponentType(MESH_COMP)->AsMesh()->SetAllMeshesMaterial(comp2->AsMaterial()->GetMaterial(0));
		}
	}

	return ret;
}


bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro scene");
	root->DeleteAllChilds();
	return true;
}

void ModuleScene::CreateEmptyGameobject()
{
	GameObject* go = new GameObject("NewGameobject");
	root->AddChild(go);
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
