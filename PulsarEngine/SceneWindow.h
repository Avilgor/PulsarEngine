#ifndef __SceneWindow_H__
#define __SceneWindow_H__

#include "EditorWindow.h"
#include "Globals.h"
#include "ImGui/imgui.h"
#include <string>

class SceneWindow : public EditorWindow
{
public:
	SceneWindow(std::string name);
	virtual ~SceneWindow();

	update_status Draw();

private:
	ImGuiWindowFlags flags;
};

#endif //__SceneWindow_H__