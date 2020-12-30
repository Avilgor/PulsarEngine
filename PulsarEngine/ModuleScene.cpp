#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ImGuizmo/ImGuizmo.h"
#include "MathGeoLib/include/MathGeoLib.h"

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 30.0f * DEGTORAD
#define BRAKE_POWER 1000.0f

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
		if (temp != nullptr)
		{
			activeScene = temp->AsScene();
			activeScene->references++;
		}
		else
		{
			activeScene = new Scene("NewScene");
			GameObject* camera = new GameObject("MainCamera");
			camera->AddComponent(CAMERA_COMP);
			activeScene->GetRoot()->AddChild(camera);
			App->resourceManager->PlaceResource(activeScene->resource);
			App->resourceManager->SaveResource(activeScene->UUID, "Assets/");
			activeScene->references++;
			LOG("New scene created");
		}
	}
	else
	{
		activeScene = new Scene("NewScene");
		GameObject* camera = new GameObject("MainCamera");
		camera->AddComponent(CAMERA_COMP);
		activeScene->GetRoot()->AddChild(camera);
		App->resourceManager->PlaceResource(activeScene->resource);
		App->resourceManager->SaveResource(activeScene->UUID,"Assets/");
		LOG("New scene created");
		activeScene->references++;
	}


	//VEHICLE

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 1, 4);
	car.chassis_offset.Set(0, 0.5, 0);
	car.front_chassis_size.Set(3, 0.5, 1);
	car.front_chassis_offset.Set(0, 0.3, 2);
	car.rear_chassis_size.Set(3.5, 0.3, 1);
	car.rear_chassis_offset.Set(0, 1, -2);
	car.cabine_radius = 0.8f;
	car.cabine_offset.Set(0, 0.9, 1);
	car.antenaOffset.Set(0, 4, -5);
	car.mass = 500.0f;
	car.suspensionStiffness = 15.0f;
	car.suspensionCompression = 2.0f;
	car.suspensionDamping = 1.0f;
	car.maxSuspensionTravelCm = 100.0f;
	car.frictionSlip = 50.5f;
	car.maxSuspensionForce = 10000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.0f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x * 0.65f;
	float half_length = car.chassis_size.z * 0.5f;

	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 2, 0);
	//vehicle->collision_listeners.add(this);
	//

	return ret;
}

bool ModuleScene::CleanUp()
{
	LOG("Unloading current scene...");
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
	if (activeScene != nullptr) App->resourceManager->FreeResource(activeScene->UUID);
	activeScene = App->resourceManager->GetResource(uuid)->AsScene();
	activeScene->references++;
}

void ModuleScene::SetScene(Scene* scene)
{
	if (activeScene != nullptr) App->resourceManager->FreeResource(activeScene->UUID);
	activeScene = scene;
	activeScene->references++;
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
				App->physics->ToggleSimulation(true);
				state = SCENE_RUNNING;
			}
			break;
		case SCENE_STOP:
			if (lastState == SCENE_PAUSE || lastState == SCENE_RUNNING)
			{
				sceneStartTimer.Stop();
				sceneRunTime.Stop();
				App->physics->ToggleSimulation(false);
				activeScene->LoadTempScene();
			}
			break;		
		case SCENE_PAUSE:
			if (lastState == SCENE_RUNNING)
			{
				sceneRunTime.Stop();
				App->physics->ToggleSimulationPause(true);
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
				App->physics->ToggleSimulationPause(false);
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

	///VEHICLE CONTROL

	turn = acceleration = brake = 0.0f;
	if (state == SCENE_RUNNING)
	{
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			acceleration = MAX_ACCELERATION * 2;
		}

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			if (turn < TURN_DEGREES)
				turn += TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			if (turn > -TURN_DEGREES)
				turn -= TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			acceleration = -MAX_ACCELERATION * 2;
		}

		if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT)
		{
			brake = BRAKE_POWER;
		}
	}


	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);
	vehicle->Render();

	/// 

	return ret;
}

// PostUpdate
update_status ModuleScene::PostUpdate(float dt)
{	
		
	return UPDATE_CONTINUE;
}
