#include "Globals.h"
#include "Application.h"
#include "InspectorWindow.h"
#include "GameObject.h"
#include "Transform.h"
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

	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::Begin(name.c_str(), &active);
	if (!App->editor->selectedGameObjects.empty())
	{
		GameObject* go = App->editor->selectedGameObjects[0];
		GameObjectSection(go);
		if (go->transform != nullptr) TransformSection(go);
		if (go->GetFirstComponentType(MESH_COMP)) MeshSection(go);
		if (go->GetFirstComponentType(MATERIAL_COMP)) MaterialSection(go);
	}
	ImGui::End();	

	return ret;
}

void InspectorWindow::GameObjectSection(GameObject* go)
{
	if (ImGui::Checkbox("##", &go->active)) go->SetActive(go->active);
	ImGui::SameLine();

	char name[50];
	strcpy_s(name, 50, go->name.c_str());
	if (ImGui::InputText("###", name, 50, name_input_flags)) go->name = name;

	ImGui::Separator();
}

void InspectorWindow::TransformSection(GameObject* go)
{
	Transform* tr = go->transform;
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		//Position
		float3 pos = tr->GetPosition();
		bool newPos = false;

		ImGui::Text("Position");

		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.20f);
		if (ImGui::InputFloat("##posx", &pos.x, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) newPos = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.20f);
		if (ImGui::InputFloat("##posy", &pos.y, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) newPos = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.20f);
		if (ImGui::InputFloat("##posz", &pos.z, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) newPos = true;
		ImGui::PopItemWidth();
		if (newPos) go->transform->SetPosition(pos);	

		//Rotation
		float3 rot = tr->GetEulerRotation();
		bool newRot = false;

		ImGui::Text("Rotation");

		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.20f);
		if (ImGui::InputFloat("##rotx", &rot.x, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) newRot = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.20f);
		if (ImGui::InputFloat("##roty", &rot.y, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) newRot = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.20f);
		if (ImGui::InputFloat("##rotz", &rot.z, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) newRot = true;
		ImGui::PopItemWidth();
		if (newRot) go->transform->SetEulerRotation(rot);

		//Scale
		float3 scale = tr->GetScale();
		bool newScale = false;

		ImGui::Text("Scale");

		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.20f);
		if (ImGui::InputFloat("##scalex", &scale.x, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) newScale = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.20f);
		if (ImGui::InputFloat("##scaley", &scale.y, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) newScale = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::Text("Z");
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.20f);
		if (ImGui::InputFloat("##scalez", &scale.z, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) newScale = true;
		ImGui::PopItemWidth();
		if (newScale) go->transform->SetScale(scale);
	}
}

void InspectorWindow::MeshSection(GameObject* go)
{
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{

	}
}

void InspectorWindow::MaterialSection(GameObject* go)
{
	if (ImGui::CollapsingHeader("Materials", ImGuiTreeNodeFlags_DefaultOpen))
	{

	}
}