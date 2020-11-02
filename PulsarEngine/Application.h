#ifndef __Application_H__
#define __Application_H__

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "EditorMain.h"
//#include "FBXLoaderModule.h"
#include "FileSystemModule.h"

#include <list> 
#include <string>


class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleScene* scene;
	EditorMain* editor;
	//FBXLoaderModule* fbxLoader;
	FileSystemModule* fileSystem;

	float ms = 0.0f;
	float fps = 0.0f;

	std::vector<std::string> ConsoleLogs;

	//int GoIDNum = 0;

private:

	float	fpsCap = 60.0f;
	Timer	ms_timer;
	float	dt;
	std::list<Module*> modulesList;
	std::string title;
	std::string organization;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	const char* GetTitleName() const;
	const char* GetOrganizationName() const;
	std::string GenerateUUID_V4();
	void SaveSettings();
	void LoadSettings();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;

#endif //__Application_H__