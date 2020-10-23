#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Transform.h"


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
	

	warrior = new GameObject("Warrior");
	warrior->AddComponent(MESH_COMP);
	warrior->transform->SetScale(float3(0.01f,0.01f,0.01f));
	warrior->transform->SetEulerRotation(float3(90.0f,0.0f,0));
	root->AddChild(warrior);

	Component* comp = warrior->GetFirstComponentType(MESH_COMP);
	if (comp != nullptr)
	{
		if (comp->AsMesh() != nullptr)
		{
			App->fbxLoader->ImportMesh(comp->AsMesh(), "Assets/3D/warrior/warrior.FBX");
		}
	}
	
	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
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

	root->UpdateTransform();
	root->UpdateGameObject();
	
	return UPDATE_CONTINUE;
}

// PostUpdate
update_status ModuleScene::PostUpdate(float dt)
{	
	return UPDATE_CONTINUE;
}
