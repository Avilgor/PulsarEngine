#include "Globals.h"
#include "Application.h"
#include "InspectorWindow.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include <string>

InspectorWindow::InspectorWindow(std::string name) : EditorWindow(name)
{}

InspectorWindow::~InspectorWindow()
{}


update_status InspectorWindow::Draw()
{
	update_status ret = UPDATE_CONTINUE;

	if (!App->editor->selectedGameObjects.empty())
	{
		GameObject* go = App->editor->selectedGameObjects[0];

		ImGui::SetNextWindowBgAlpha(1.0f);
		ImGui::Begin(name.c_str(), &active);
		GameObjectSection();

		if (go->GetFirstComponentType(TRANSFORM_COMP)) TransformSection();
		if (go->GetFirstComponentType(MESH_COMP)) MeshSection();
		if (go->GetFirstComponentType(MATERIAL_COMP)) MaterialSection();
		ImGui::End();
	}

	return ret;
}

void InspectorWindow::GameObjectSection()
{

}

void InspectorWindow::TransformSection()
{

}

void InspectorWindow::MeshSection()
{

}

void InspectorWindow::MaterialSection()
{

}