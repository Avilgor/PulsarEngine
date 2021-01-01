#ifndef __SceneWindow_H__
#define __SceneWindow_H__

#include "EditorWindow.h"
#include "Globals.h"
#include "ImGui/imgui.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "ImGuizmo/ImGuizmo.h"

#include <string>

class SceneWindow : public EditorWindow
{
public:
	SceneWindow(std::string name);
	virtual ~SceneWindow();

	update_status Draw();
	void SetNewSize(float x, float y);
	void HandleClick();
	void HandleGuizmo();

private:
	ImGuiWindowFlags flags;
	float2 corners;
	float2 windowSize;
	float2 mousePos;

	ImGuizmo::OPERATION gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	ImGuizmo::MODE gizmoMode = ImGuizmo::MODE::LOCAL;
	
	ImVec4 pressColor{ 0.0f, 0.5f, 0.0f, 1.0f };
};

#endif //__SceneWindow_H__