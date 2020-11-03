#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Scene.h"
#include "GameObject.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app,"Scene",start_enabled)
{}

ModuleScene::~ModuleScene()
{}

bool ModuleScene::Init()
{	
	activeScene = new Scene("SampleScene");
	return true;
}

// Load assets
bool ModuleScene::Start()
{
	LOG("Starting module scene...");
	bool ret = true;	
	activeScene->StartScene();
	return ret;
}


bool ModuleScene::CleanUp()
{
	LOG("Unloading current scene...");
	activeScene->CleanScene();
	return true;
}

Scene* ModuleScene::GetActiveScene()
{
	return activeScene;
}

void ModuleScene::SaveCurrentScene()
{
	activeScene->SaveScene();
}

void ModuleScene::LoadNewScene()
{
	LOG("Loading new scene...");
	activeScene->LoadScene();
}


// PreUpdate
update_status ModuleScene::PreUpdate(float dt)
{	
	return UPDATE_CONTINUE;
}

// Update
update_status ModuleScene::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;
	ret = activeScene->UpdateScene();
	return ret;
}

// PostUpdate
update_status ModuleScene::PostUpdate(float dt)
{	
		
	return UPDATE_CONTINUE;
}
