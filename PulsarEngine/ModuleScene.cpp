#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Scene.h"
#include "GameObject.h"


ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app,"Scene",start_enabled)
{
	save = false;
	load = false;
}

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
	LoadScene();
	return ret;
}


bool ModuleScene::CleanUp()
{
	LOG("Unloading current scene...");
	//activeScene->SaveScene();
	activeScene->CleanScene();
	return true;
}

Scene* ModuleScene::GetActiveScene()
{
	return activeScene;
}

void ModuleScene::RequestSave()
{
	save = true;
}

void ModuleScene::RequestLoad()
{
	load = true;
}

void ModuleScene::SaveScene()
{
	activeScene->SaveScene();
	save = false;
}

void ModuleScene::LoadScene()
{
	LOG("Loading new scene...");

	Scene* scene = new Scene();
	char* buffer = nullptr;
	std::string path = SCENES_PATH;
	path.append("SampleScene.JSON");
	uint size = App->fileSystem->Load(path.c_str(), &buffer);
	if (size > 0)
	{
		//activeScene->SaveScene();
		activeScene->CleanScene();

		JSonHandler* node = new JSonHandler(buffer);
		activeScene = scene;
		activeScene->LoadScene(node);
		load = false;

		delete node;
		RELEASE_ARRAY(buffer);
	}
	else LOG("Error loading scene.");
}

void ModuleScene::CreateNewScene()
{
	activeScene->SaveScene();
	activeScene->CleanScene();
	Scene* scenenew = new Scene("NewScene");
	activeScene = scenenew;
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
	if (save) SaveScene();
	if (load) LoadScene();
		
	return ret;
}

// PostUpdate
update_status ModuleScene::PostUpdate(float dt)
{	
		
	return UPDATE_CONTINUE;
}
