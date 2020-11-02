#pragma once

#include "JSonHandler.h"
#include <string>

class Application;

class Module
{
private :
	bool enabled;

public:
	Application* App;
	std::string name;

	Module(Application* parent, const char* n, bool start_enabled = true) : App(parent), name(n)
	{}

	virtual ~Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void SaveSettings(JSonHandler node)
	{}

	virtual void LoadSettings(JSonHandler node)
	{}
};