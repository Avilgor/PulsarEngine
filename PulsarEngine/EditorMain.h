#ifndef __EditorMain_H__
#define __EditorMain_H__

#include "Module.h"
#include "Globals.h"


class EditorMain : public Module
{
public:

	EditorMain(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~EditorMain();

	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();


public:
	SDL_Window* mainWindow;
};

#endif // __EditorMain_H__