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

    ImGui::Begin(name.c_str(), &active,flags);

    ImGui::End();

	return ret;
}