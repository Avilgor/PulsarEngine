#ifndef __EditorMain_H__
#define __EditorMain_H__

#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "EditorWindow.h"
#include "ImGui/imgui.h"

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

	update_status RenderDock();
	void ProccesInput(SDL_Event e);
	void SetWindowsActive(int index,bool val);
	void SelectOne(GameObject* go);
	void AddSelection(GameObject* go);
	void RemoveSelection(GameObject* go);
	void CreateSelectionChild();
	void DeleteSelectionChilds();
	void DeleteSelected();
	void EmptySelected();
	bool HasSelection();

private:
	std::vector<EditorWindow*> WindowsList;
	bool dock = true;
	ImGuiWindowFlags dockFlags;
	ImGuiDockNodeFlags dockspace_flags;

public:
	SDL_Window* mainWindow;	
	bool mouse_in_scene = false;
	std::vector<GameObject*> selectedGameObjects;

	KEY_STATE rightMouse;
	KEY_STATE leftMouse;
	KEY_STATE ctrl;
	KEY_STATE deleteKey;
	bool mouseDrag = false;
	bool scanProjectFiles = true;
};

#endif // __EditorMain_H__