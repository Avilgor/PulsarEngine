#include "Application.h"


Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	scene = new ModuleScene(this);
	editor = new EditorMain(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	
	// Scenes
	AddModule(scene); 
	AddModule(editor);

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
		if(ret == UPDATE_CONTINUE) ret = (*it)->PreUpdate(dt);
	}

	for (std::list<Module*>::const_iterator it = modulesList.begin(); it != modulesList.end(); ++it)
	{
		if(ret == UPDATE_CONTINUE) ret = (*it)->Update(dt);
	}

	for (std::list<Module*>::const_iterator it = modulesList.begin(); it != modulesList.end(); ++it)
	{
		if(ret == UPDATE_CONTINUE) ret = (*it)->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
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