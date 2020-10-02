#ifndef __EditorMain_H__
#define __EditorMain_H__

#include "Module.h"
#include "Globals.h"
#include "EditorWindow.h"
#include <vector>


class EditorMain : public Module
{
public:

	EditorMain(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~EditorMain();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void ProccesInput(SDL_Event e);

private:
	std::vector<EditorWindow*> WindowsList;

public:
	SDL_Window* mainWindow;	
};

#endif // __EditorMain_H__