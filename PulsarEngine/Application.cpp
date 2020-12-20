#include "Application.h"
#include "JSonHandler.h"

#include <random>
#include <sstream>

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<> dis(0, 15);
static std::uniform_int_distribution<> dis2(8, 11);


Application::Application()
{
	title = "Sample";
	organization = "Unknown";
	fileSystem = new FileSystemModule(this);
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	scene = new ModuleScene(this);
	editor = new EditorMain(this);
	resourceManager = new ResourceManager(this);
	physics = new ModulePhysics(this);

	// Main Modules
	AddModule(fileSystem);
	AddModule(resourceManager);
	AddModule(window);
	AddModule(camera);
	AddModule(input);

	// Scenes
	AddModule(editor);
	AddModule(scene);		
	AddModule(physics);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	for (std::list<Module*>::const_iterator it = modulesList.begin(); it != modulesList.end(); ++it)
	{
		delete (*it);
	}
}

bool Application::Init()
{
	bool ret = true;
	// Call Init() in all modules
	for (std::list<Module*>::const_iterator it = modulesList.begin(); it != modulesList.end(); ++it)
	{
		if(ret) ret = (*it)->Init();
	}
	LoadSettings();
	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	for (std::list<Module*>::const_iterator it = modulesList.begin(); it != modulesList.end(); ++it)
	{
		if(ret) ret = (*it)->Start();
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	ms = ms_timer.Read();
	if (ms > 0 && ms < (1000/ fpsCap))
	{
		SDL_Delay((1000/ fpsCap) - ms);
	}
	fps = 1000 / (float)ms_timer.Read();
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	for (std::list<Module*>::const_iterator it = modulesList.begin(); it != modulesList.end(); ++it)
	{ 
		//LOG("Module %s preupdate",(*it)->name.c_str());
		if(ret == UPDATE_CONTINUE) ret = (*it)->PreUpdate(dt);
	}

	for (std::list<Module*>::const_iterator it = modulesList.begin(); it != modulesList.end(); ++it)
	{
		//LOG("Module %s update", (*it)->name.c_str());
		if(ret == UPDATE_CONTINUE) ret = (*it)->Update(dt);
	}

	for (std::list<Module*>::const_iterator it = modulesList.begin(); it != modulesList.end(); ++it)
	{
		//LOG("Module %s postupdate", (*it)->name.c_str());
		if(ret == UPDATE_CONTINUE) ret = (*it)->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	if (scene->GetActiveScene() != nullptr) resourceManager->SaveResource(scene->GetActiveSceneUUID());
	SaveSettings();
	for (std::list<Module*>::const_iterator it = modulesList.begin(); it != modulesList.end(); ++it)
	{ 
		if (ret) ret = (*it)->CleanUp();
		else break;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	modulesList.push_back(mod);
}

const char* Application::GetTitleName() const
{
	return title.c_str();
}

const char* Application::GetOrganizationName() const
{
	return organization.c_str();
}

void Application::SaveSettings()
{
	LOG("Saving settings...");
	JSonHandler settings;
	JSonHandler node = settings.CreateNode("EngineSettings");
	
	for (std::list<Module*>::const_iterator it = modulesList.begin(); it != modulesList.end(); ++it)
	{
		(*it)->SaveSettings(node.CreateNode((*it)->name.c_str()));
	}

	char* buffer = nullptr;
	uint size = settings.Serialize(&buffer);

	fileSystem->Save("Settings.JSON", buffer, size);
	RELEASE_ARRAY(buffer);
}

void Application::LoadSettings()
{
	LOG("Loading settings...");
	char* buffer = nullptr;
	uint size = fileSystem->Load("Settings.JSON", &buffer);

	if (size > 0)
	{
		//LOG("Size ok");
		JSonHandler file(buffer);
		JSonHandler settings = file.GetNode("EngineSettings");

		for (std::list<Module*>::const_iterator it = modulesList.begin(); it != modulesList.end(); ++it)
		{
			LOG("Loding %s settings..",(*it)->name.c_str());
			(*it)->LoadSettings(settings.GetNode((*it)->name.c_str()));
		}		
	}
}


std::string Application::GenerateUUID_V4()
{
	std::stringstream ss;
	int i;
	ss << std::hex;
	for (i = 0; i < 8; i++) {
		ss << dis(gen);
	}
	ss << "-";
	for (i = 0; i < 4; i++) {
		ss << dis(gen);
	}
	ss << "-4";
	for (i = 0; i < 3; i++) {
		ss << dis(gen);
	}
	ss << "-";
	ss << dis2(gen);
	for (i = 0; i < 3; i++) {
		ss << dis(gen);
	}
	ss << "-";
	for (i = 0; i < 12; i++) {
		ss << dis(gen);
	};
	return ss.str();
}