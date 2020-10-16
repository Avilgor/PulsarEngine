#include "Globals.h"
#include "Application.h"
#include "SceneWindow.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include <string>

SceneWindow::SceneWindow(std::string name) : EditorWindow(name)
{
	flags = /*ImGuiWindowFlags_NoBackground |*/ ImGuiWindowFlags_NoScrollbar;
}

SceneWindow::~SceneWindow()
{}


update_status SceneWindow::Draw()
{
	update_status ret = UPDATE_CONTINUE;
    ImGui::Begin(name.c_str(), &active,flags);	
	ImVec2 winSize = ImGui::GetWindowSize();
	ImGui::Image((ImTextureID)App->renderer3D->renderTexture, ImVec2(winSize.x, winSize.y), ImVec2(0, 1), ImVec2(1, 0));
	App->editor->mouse_in_scene = ImGui::IsItemHovered();	
    ImGui::End();

	return ret;
}
