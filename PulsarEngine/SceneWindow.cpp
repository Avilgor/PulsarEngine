#include "SceneWindow.h"
#include "Globals.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include <string>

SceneWindow::SceneWindow(std::string name) : EditorWindow(name)
{
	flags = ImGuiWindowFlags_NoBackground;
}

SceneWindow::~SceneWindow()
{}


update_status SceneWindow::Draw()
{
	update_status ret = UPDATE_CONTINUE;
	//ImGui::Image((ImTextureID)App->renderer3D->renderTexture, ImVec2(img_size.x, img_size.y), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::Begin(name.c_str(), &active,flags);

    ImGui::End();

	return ret;
}