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
	

	/*warrior = new GameObject("Warrior");
	warrior->AddComponent(MESH_COMP);
	warrior->AddComponent(MATERIAL_COMP);
	root->AddChild(warrior);
	warrior->transform->SetScale(float3(0.01f,0.01f,0.01f));
	warrior->transform->SetEulerRotation(float3(-90.0f,0.0f,0));
	warrior->transform->SetPosition(float3(2.0f, 0, 0));*/

	GameObject* go2 = new GameObject("Baker House");
	go2->AddComponent(MESH_COMP);
	go2->AddComponent(MATERIAL_COMP);
	root->AddChild(go2);
	//go2->transform->SetPosition(float3(-2.0f, 0, 0));


	/*Component* comp = warrior->GetFirstComponentType(MESH_COMP);
	if (comp != nullptr)
	{
		if (comp->AsMesh() != nullptr) App->fbxLoader->ImportMesh(comp->AsMesh(), "Assets/3D/warrior/warrior.FBX");		
	}

	comp = warrior->GetFirstComponentType(MATERIAL_COMP);
	if (comp != nullptr)
	{
		if (comp->AsMaterial() != nullptr)
		{
			App->fbxLoader->ImportMaterial(comp->AsMaterial(), "Assets/3D/warrior/warrior.FBX");
			warrior->GetFirstComponentType(MESH_COMP)->AsMesh()->material = comp->AsMaterial();
		}
	}*/
	
	Component* comp2 = go2->GetFirstComponentType(MESH_COMP);
	if (comp2 != nullptr)
	{
		if (comp2->AsMesh() != nullptr) App->fbxLoader->ImportMesh(comp2->AsMesh(), "Assets/3D/Baker/BakerHouse.fbx");
	}

	/*comp2 = go2->GetFirstComponentType(MATERIAL_COMP);
	if (comp2 != nullptr)
	{
		if (comp2->AsMaterial() != nullptr)
		{
			App->fbxLoader->ImportMaterial(comp2->AsMaterial(), "Assets/3D/Baker/BakerHouse.FBX");
			go2->GetFirstComponentType(MESH_COMP)->AsMesh()->SetAllMeshesMaterial(comp2->AsMaterial()->GetMaterial(0));
		}
	}*/

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
	//warrior->transform->Rotate(float3(1.0f,0,0));

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
