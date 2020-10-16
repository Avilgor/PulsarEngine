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
	ImVec2 windowsSize = { 0, 0 }; 
	ImVec2 img_corner;
	ImVec2 img_size;
	ImVec2 cornerPos;
	ImVec2 win_size;
	ImVec2 img_offset;
};

#endif //__SceneWindow_H__