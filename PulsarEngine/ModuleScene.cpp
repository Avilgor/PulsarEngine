#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "ImGuizmo/ImGuizmo.h"
#include "MathGeoLib/include/MathGeoLib.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app,"Scene",start_enabled)
{
	save = false;
	load = false;
}

ModuleScene::~ModuleScene()
{}

bool ModuleScene::Init()
{	
	state = SCENE_STOP;
	lastState = state;
	ImGuizmo::Enable(true);
	return true;
}

// Load assets
bool ModuleScene::Start()
{
	LOG("Starting module scene...");
	bool ret = true;	
	timeScale = 1.0f;
	state = SCENE_STOP;
	if (lastSceneID.compare("0") != 0)
	{
		EngineResource* temp = App->resourceManager->GetResource(lastSceneID);
		if(temp != nullptr) activeScene = temp->AsScene();
		else
		{
			activeScene = new Scene("NewScene");
			App->resourceManager->PlaceResource(activeScene->resource);
			App->resourceManager->SaveResource(activeScene->UUID, "Assets/");
			//activeScene->references++;
			LOG("New scene created");
		}
	}
	else
	{
		activeScene = new Scene("NewScene");
		App->resourceManager->PlaceResource(activeScene->resource);
		App->resourceManager->SaveResource(activeScene->UUID,"Assets/");
		LOG("New scene created");
		//->references++;
	}
	//activeScene->StartScene();
	//LoadScene();
	return ret;
}

bool ModuleScene::CleanUp()
{
	LOG("Unloading current scene...");
	//activeScene->SaveScene();
	//activeScene->SaveResource(nullptr);
	return true;
}

Scene* ModuleScene::GetActiveScene()
{
	return activeScene;
}

void ModuleScene::RequestSave()
{
	if (state == SCENE_STOP) save = true;
}

void ModuleScene::SetScene(std::string uuid)
{
	//if (activeScene != nullptr) App->resourceManager->FreeResource(activeScene->UUID);
	activeScene = App->resourceManager->GetResource(uuid)->AsScene();
	//activeScene->references++;
}

void ModuleScene::SetScene(Scene* scene)
{
	//if (activeScene != nullptr) App->resourceManager->FreeResource(activeScene->UUID);
	activeScene = scene;
	//activeScene->references++;
}

void ModuleScene::SaveScene()
{
	App->resourceManager->SaveResource(activeScene->UUID);
	save = false;
}

std::string ModuleScene::GetActiveSceneUUID()
{
	if (activeScene != nullptr) return activeScene->UUID;
	else return "0";
}

void ModuleScene::GetIntersectedGameobjects(std::vector<GameObject*>* vec, LineSegment ray)
{
	activeScene->GetRoot()->CheckRayIntersect(vec,ray);
}

void ModuleScene::SaveSettings(JSonHandler node)
{
	node.SaveString("LastScene", GetActiveSceneUUID().c_str());
}

void ModuleScene::LoadSettings(JSonHandler node)
{
	lastSceneID = node.GetString("LastScene");
	//LOG("Last scene id %s",lastSceneID.c_str());
}

void ModuleScene::SetTimeScale(float val)
{
	timeScale = val;
}

float ModuleScene::GetDeltaTime()
{
	return lastDT * timeScale;
}

float ModuleScene::GetSceneRunningTime()
{
	return sceneRunTime.ReadSec();
}

void ModuleScene::CreateNewScene()
{
	App->resourceManager->SaveResource(activeScene->UUID);
	App->resourceManager->FreeResource(activeScene->UUID);
	activeScene = new Scene("NewScene");
	App->resourceManager->PlaceResource(activeScene->resource);
	activeScene->StartScene();
	App->resourceManager->SaveResource(activeScene->UUID);
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

	if (state != lastState)
	{
		switch (state)
		{
		case SCENE_PLAY:
			if (lastState == SCENE_STOP)
			{
				sceneStartTimer.Start();
				sceneRunTime.Start();
				activeScene->SaveTempScene();
				state = SCENE_RUNNING;
			}
			break;
		case SCENE_STOP:
			if (lastState == SCENE_PAUSE || lastState == SCENE_RUNNING)
			{
				sceneStartTimer.Stop();
				sceneRunTime.Stop();
				activeScene->LoadTempScene();
			}
			break;		
		case SCENE_PAUSE:
			if (lastState == SCENE_RUNNING)
			{
				sceneRunTime.Stop();
			}
			break;
		case SCENE_RUNNING:
			if (lastState == SCENE_UNPAUSE || lastState == SCENE_PLAY)
			{

			}
			break;
		case SCENE_UNPAUSE:
			if (lastState == SCENE_PAUSE)
			{
				sceneRunTime.Resume();
				state = SCENE_RUNNING;
			}
			break;
		case SCENE_STEP:
			if (lastState == SCENE_PAUSE)
			{
				sceneRunTime.Resume();
				state = SCENE_ENDSTEP;
			}
			break;
		case SCENE_ENDSTEP:
			if (lastState == SCENE_STEP)
			{
				state = SCENE_PAUSE;
			}
			break;
		}				
	}

	ret = activeScene->UpdateScene(dt);

	if (save) SaveScene();

	if (state != lastState) lastState = state;
	lastDT = dt;
	return ret;
}

// PostUpdate
update_status ModuleScene::PostUpdate(float dt)
{	
		
	return UPDATE_CONTINUE;
}
