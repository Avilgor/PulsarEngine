#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Scene.h"
#include "GameObject.h"
#include "Mesh.h"
#include "RES_Mesh.h"
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
	activeScene = new Scene("SampleScene");
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

void ModuleScene::GetIntersectedGameobjects(std::vector<GameObject*>* vec, LineSegment ray)
{
	activeScene->GetRoot()->CheckRayIntersect(vec,ray);
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
	if (load) LoadScene();

	if (state != lastState) lastState = state;
	lastDT = dt;
	return ret;
}

// PostUpdate
update_status ModuleScene::PostUpdate(float dt)
{	
		
	return UPDATE_CONTINUE;
}
