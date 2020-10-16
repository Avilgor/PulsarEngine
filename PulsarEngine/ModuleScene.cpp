#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
//#include "Mesh.h"


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

    //cube = new Mesh();
    //cube->CreateCube(0.0f,1.0f,0.0f);
	Warrior = new Mesh();
	App->fbxLoader->ImportMesh("Assets/3D/warrior/warrior.FBX");
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
    //cube->Render();
	Warrior->Render();

	return UPDATE_CONTINUE;
}

// PostUpdate
update_status ModuleScene::PostUpdate(float dt)
{	
	return UPDATE_CONTINUE;
}
