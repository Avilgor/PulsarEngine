#include "Globals.h"
#include "Application.h"
#include "SceneWindow.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include <string>

SceneWindow::SceneWindow(std::string name) : EditorWindow(name)
{
	flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize;
	lastSizeX = 0;
	lastSizeY = 0;
	cornerX = 0;
	cornerY = 0;
	windowSizeX = 0;
	windowSizeY = 0;
	offsetX = 0;
	offsetY = 0;
}

SceneWindow::~SceneWindow()
{}


update_status SceneWindow::Draw()
{
	update_status ret = UPDATE_CONTINUE;
    ImGui::Begin(name.c_str(), &active,flags);	
	ImVec2 winSize = ImGui::GetWindowSize();	
	if (winSize.x != lastSizeX || winSize.y != lastSizeY) SetNewSize(winSize.x, winSize.y);	
	ImGui::SetCursorPos(ImVec2(offsetX, offsetY));	
	cornerX = ImGui::GetCursorScreenPos().x;
	cornerY = ImGui::GetCursorScreenPos().y + windowSizeY;
	cornerY = App->window->height - cornerY;
	ImGui::Image((ImTextureID)App->renderer3D->renderTexture, winSize);	
	App->editor->mouse_in_scene = ImGui::IsItemHovered();
    ImGui::End();

	return ret;
}

void SceneWindow::SetNewSize(float x, float y)
{
	lastSizeX = x;
	lastSizeY = y;

	windowSizeX = App->window->width;
	windowSizeY = App->window->height;

	if (windowSizeX > lastSizeX - 10.0f)
	{
		windowSizeX /= (windowSizeX / (lastSizeX - 10.0f));
		windowSizeY /= (windowSizeX / (lastSizeX - 10.0f));
	}

	if (windowSizeY > lastSizeY - 10.0f)
	{
		windowSizeX /= (windowSizeX / (lastSizeY - 10.0f));
		windowSizeY /= (windowSizeX / (lastSizeY - 10.0f));
	}
	offsetX = (lastSizeX - 5.0f - windowSizeX) / 2; 
	offsetY = (lastSizeY - 5.0f - windowSizeY) / 2;

}