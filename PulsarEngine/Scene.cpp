#include "Globals.h"
#include "Application.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "JSonHandler.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ResourceManager.h"

#include <map>

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 30.0f * DEGTORAD
#define BRAKE_POWER 1000.0f

Scene::Scene() : EngineResource(SCENE_RES)
{
	root = new GameObject("Root");
	root->SetUUID("0");
	sceneRes = this;
}

Scene::Scene(const char* n) : EngineResource(SCENE_RES)
{
	name = n;
	root = new GameObject("Root");
	root->SetUUID("0");
	sceneRes = this;
}

Scene::Scene(std::string id) : EngineResource(SCENE_RES,id)
{
	root = new GameObject("Root");
	root->SetUUID("0");
	sceneRes = this;
}

Scene::Scene(const char* n, std::string id) : EngineResource(SCENE_RES, id)
{
	name = n;
	root = new GameObject("Root");
	root->SetUUID("0");
	sceneRes = this;
}

Scene::~Scene()
{
	if (root != nullptr)
	{
		root->DeleteAllChilds();
		root->Delete();
	}
}

void Scene::StartScene()
{	
	LOG("Starting scene %s",name.c_str());
	//VEHICLE
	AddCar();
	//

	cameraCollider = new GameObject("Camera");
	Component* coll = cameraCollider->AddComponent(SPHERE_COLLIDER_COMP);
	if (coll != nullptr)
	{
		cameraCollider->transform->ForceGlobalPos(App->camera->GetPos());
		coll->AsSphereCollider()->SetStatic(true);
	}
	else cameraCollider->DeleteGameobject();
}

void Scene::AddCar()
{
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
}

update_status Scene::PreUpdateScene(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (cameraCollider != nullptr) cameraCollider->transform->ForceGlobalPos(App->camera->GetPos());
	/*LOG("Collider cam pos: %f/%f/%f", 
		cameraCollider->transform->GetGlobalPosition().x, 
		cameraCollider->transform->GetGlobalPosition().y,
		cameraCollider->transform->GetGlobalPosition().z);*/
	return ret;
}

void Scene::StartRecoveringScene()
{
	recoveringScene = true;
	recoverSteps = 0;
}

update_status Scene::UpdateScene(float dt)
{
	update_status ret = UPDATE_CONTINUE;
	//LOG("Scene update");
	App->renderer3D->RenderGroundGrid(10);

	if (recoveringScene)
	{
		LoadTempScene();
		recoverSteps++;
	}

	if (root != nullptr)
	{
		root->UpdateTransform();
		root->UpdateGameObject();

		root->DrawMesh();
	}
	
	///VEHICLE CONTROL
	/// 
	if (!recoveringScene)
	{
		turn = acceleration = brake = 0.0f;
		if (App->physics->runningSimulation)
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

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				ThrowPhysBall();
			}

			if (vehicle != nullptr)
			{
				vehicle->ApplyEngineForce(acceleration);
				vehicle->Turn(turn);
				vehicle->Brake(brake);
			}
		}
		if (vehicle != nullptr) vehicle->Render();
	}
	/// 

	return ret;
}

update_status Scene::PostUpdateScene(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	return ret;
}

void Scene::Clean()
{
	App->editor->EmptySelected();
	root->DeleteAllChilds();
	root->Delete();
	root = nullptr;
	delete this;
}

void Scene::ThrowPhysBall()
{
	GameObject* go = new GameObject("Ball");
	App->resourceManager->ImportFBX("DefaultAssets/Models/sphere.fbx", go);
	GameObject* ball = go->GetFirstChild();
	if (ball != nullptr)
	{
		ball->name = "Ball";
		ball->SetParent(root);
		Component* coll = ball->AddComponent(SPHERE_COLLIDER_COMP);
		if (coll != nullptr)
		{
			ball->transform->SetPosition(App->camera->GetPos());
			coll->AsSphereCollider()->ApplyForce(App->camera->GetEditorDirection(), 300);
			balls.push_back(ball);
		}
		else ball->DeleteGameobject();
	}
	go->DeleteGameobject();
}

void Scene::ClearBalls()
{
	if (!balls.empty())
	{
		for (int i = 0; i < balls.size(); i++) balls[i]->DeleteGameobject();
		balls.clear();
	}
}


void Scene::SaveResource(JSonHandler* file)
{	
	//Save gameobjects 
	file->SaveString("UUID", UUID.c_str());
	file->SaveNum("Type", (double)type);
	file->SaveString("Name",name.c_str());
	std::string label = "Gameobjects";
	file->CreateArray(label.c_str());
	root->SaveGameobject(file,label.c_str());

 	LOG("Scene %s saved!", name.c_str());
}

bool Scene::LoadScene(JSonHandler* file)
{
	bool ret = true;
	std::map <std::string, GameObject*> gameobjectsLoaded;
	if (file != nullptr)
	{
		if (file->LoadArray("Gameobjects"))
		{
			int num = file->GetArrayCount("Gameobjects");
			gameobjectsLoaded.emplace(root->UUID, root);

			if (num > 1)//Start with 1 to avoid root
			{
				//Load all gameobjects
				for (int a = 1; a < num; a++)
				{
					GameObject* go = new GameObject();
					JSonHandler temp = file->GetNodeArray("Gameobjects", a);
					go->LoadGameObject(&temp);
					gameobjectsLoaded.emplace(go->UUID, go);
				}

				//Set gameobjects hierarchy
				for (std::map<std::string, GameObject*>::iterator it = gameobjectsLoaded.begin(); it != gameobjectsLoaded.end(); it++)
				{
					if ((*it).second->UUID.compare("0") != 0 && (*it).second->parentUUID.compare("") != 0)
					{
						gameobjectsLoaded[(*it).second->parentUUID]->AddChild((*it).second);
						(*it).second->SetParent(gameobjectsLoaded[(*it).second->parentUUID]);
					}
				}
			}
			gameobjectsLoaded.clear();
			LOG("Scene %s loaded!", name.c_str());
		}
		else
		{
			LOG("Scene gameobjects array error.");
			ret = false;
		}
	}
	else
	{
		LOG("Scene json error");
		ret = false;
	}
	return ret;
}

void Scene::LoadResource(JSonHandler* file)
{
	UUID = file->GetString("UUID");
	name = file->GetString("Name");
}

void Scene::SaveTempScene()
{
	//Save gameobjects hierarchy
	JSonHandler settings;
	settings.SaveString("Name", name.c_str());
	std::string label = "Gameobjects";
	settings.CreateArray(label.c_str());
	root->SaveGameobject(&settings, label.c_str());

	//Write to file
	char* buffer = nullptr;
	uint size = settings.Serialize(&buffer);
	std::string fileName = SCENES_PATH;
	fileName.append(name);
	fileName.append("_temp.psscene");
	App->fileSystem->Save(fileName.c_str(), buffer, size);
	RELEASE_ARRAY(buffer);

	//LOG("Recover scene created", name.c_str());
}

void Scene::LoadTempScene()
{
	Component* coll = nullptr;
	switch (recoverSteps)
	{
	case 0:
		App->editor->EmptySelected();
		root->DeleteAllChilds();
		if (cameraCollider != nullptr) cameraCollider->DeleteGameobject();
		break;
	case 1:
		App->physics->ResetPhysics();
		break;
	case 2:
		AddCar();
		if (cameraCollider != nullptr) cameraCollider->DeleteGameobject();
		cameraCollider = new GameObject("Camera");
		coll = cameraCollider->AddComponent(SPHERE_COLLIDER_COMP);
		if (coll != nullptr)
		{
			cameraCollider->transform->SetPosition(App->camera->GetPos());
			coll->AsSphereCollider()->SetStatic(true);
		}
		else cameraCollider->DeleteGameobject();
		break;
	case 3:
		char* buffer = nullptr;
		std::string path = SCENES_PATH;
		path.append(name);
		path.append("_temp.psscene");
		uint size = App->fileSystem->Load(path.c_str(), &buffer);
		if (size > 0)
		{
			JSonHandler* node = new JSonHandler(buffer);
			LoadScene(node);
			delete node;
			RELEASE_ARRAY(buffer);
		}
		else LOG("Error recovering scene.");
		recoveringScene = false;
		break;
	}	
}

GameObject* Scene::CreateEmptyGameobject()
{
	GameObject* go = new GameObject("NewGameobject");
	root->AddChild(go);
	return go;
}

GameObject* Scene::CreateEmptyGameobject(const char* name)
{
	GameObject* go = new GameObject(name);
	root->AddChild(go);
	return go;
}

void Scene::CreateCube()
{
	GameObject* go = new GameObject("Cube");
	root->AddChild(go);
	App->resourceManager->ImportFBX("DefaultAssets/Models/cube.fbx", go);
}

void Scene::CreatePyramid()
{
	GameObject* go = new GameObject("Pyramid");
	root->AddChild(go);
	App->resourceManager->ImportFBX("DefaultAssets/Models/cone.fbx", go);
}

void Scene::CreatePlane()
{
	GameObject* go = new GameObject("Plane");
	root->AddChild(go);
	App->resourceManager->ImportFBX("DefaultAssets/Models/plane.fbx", go);
}

void Scene::CreateSphere()
{
	GameObject* go = new GameObject("Sphere");
	root->AddChild(go);
	App->resourceManager->ImportFBX("DefaultAssets/Models/sphere.fbx", go);
}

void Scene::CreateCylinder()
{
	GameObject* go = new GameObject("Cylinder");
	root->AddChild(go);
	App->resourceManager->ImportFBX("DefaultAssets/Models/cylinder.fbx", go);
}

void Scene::CreateTorus()
{
	GameObject* go = new GameObject("Torus");
	root->AddChild(go);
	App->resourceManager->ImportFBX("DefaultAssets/Models/torus.fbx", go);
}