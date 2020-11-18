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
	float2 SceneToWindow(float2 p);
	float2 WindowToScene(float2 p);

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

	ImGuizmo::OPERATION gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	ImGuizmo::MODE gizmoMode = ImGuizmo::MODE::WORLD;

	float3 nearClick;
	float3 farClick;
};

#endif //__SceneWindow_H__