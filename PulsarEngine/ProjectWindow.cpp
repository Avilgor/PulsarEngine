#include "ProjectWindow.h"
#include "Globals.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include <string>

ProjectWindow::ProjectWindow(std::string name) : EditorWindow(name)
{}

ProjectWindow::~ProjectWindow()
{}


update_status ProjectWindow::Draw()
{
	update_status ret = UPDATE_CONTINUE;

	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::Begin(name.c_str(), &active);

	ImGui::End();

	return ret;
}