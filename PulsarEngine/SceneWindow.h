#ifndef __SceneWindow_H__
#define __SceneWindow_H__

#include "EditorWindow.h"
#include "Globals.h"
#include "ImGui/imgui.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include <string>

class SceneWindow : public EditorWindow
{
public:
	SceneWindow(std::string name);
	virtual ~SceneWindow();

	update_status Draw();
	void SetNewSize(float x, float y);
	void ClickSelect(ImVec2 size);

private:
	ImGuiWindowFlags flags;
	float lastSizeX;
	float lastSizeY;
	float cornerX;
	float cornerY;
	float windowSizeX;
	float windowSizeY;
	float offsetX;
	float offsetY;
};

#endif //__SceneWindow_H__