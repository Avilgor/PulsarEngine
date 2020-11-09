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
{
	LOG("Delete scene");
}


update_status SceneWindow::Draw()
{
	update_status ret = UPDATE_CONTINUE;
    ImGui::Begin(name.c_str(), &active,flags);
	App->editor->mouse_in_scene = ImGui::IsWindowHovered();
	ImVec2 winSize = ImGui::GetWindowSize();

	ImGui::Indent((winSize.x/2) -(75 *2));
	if(ImGui::Button("Play",ImVec2(75.0f,25.0f))) App->scene->state = SCENE_PLAY;
	ImGui::SameLine();
	if (ImGui::Button("Pause", ImVec2(75.0f, 25.0f))) App->scene->state = SCENE_PAUSE;
	ImGui::SameLine();
	if (ImGui::Button("Stop", ImVec2(75.0f, 25.0f))) App->scene->state = SCENE_STOP;
	ImGui::SameLine();
	if (ImGui::Button("Step", ImVec2(75.0f, 25.0f))) App->scene->state = SCENE_STEP;
	ImGui::Unindent();
	
		
	if (winSize.x != lastSizeX || winSize.y != lastSizeY) SetNewSize(winSize.x, winSize.y);	
	ImGui::SetCursorPos(ImVec2(offsetX, offsetY));	
	cornerX = ImGui::GetCursorScreenPos().x;
	cornerY = ImGui::GetCursorScreenPos().y + windowSizeY;
	cornerY = App->window->height - cornerY;
	ImGui::Image((ImTextureID)App->renderer3D->renderTexture, winSize);		
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