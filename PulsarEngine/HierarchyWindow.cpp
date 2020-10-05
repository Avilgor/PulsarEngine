#include "HierarchyWindow.h"
#include "Globals.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include <string>

HierarchyWindow::HierarchyWindow(std::string name) : EditorWindow(name)
{}

HierarchyWindow::~HierarchyWindow()
{}


update_status HierarchyWindow::Draw()
{
	update_status ret = UPDATE_CONTINUE;

	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::Begin(name.c_str(), &active);

	ImGui::End();

	return ret;
}