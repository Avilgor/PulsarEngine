#include "Globals.h"
#include "Application.h"
#include "InspectorWindow.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "RES_Mesh.h"
#include "RES_Material.h"
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
	if (ImGui::Checkbox("AABB", &go->drawAABB)) go->SetDrawAABB(go->drawAABB);
}

void InspectorWindow::TransformSection(GameObject* go)
{
	Transform* tr = go->transform;
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		//Position
		float3 pos = tr->GetLocalPosition();
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
		Mesh* mesh = go->GetFirstComponentType(MESH_COMP)->AsMesh();
		ImGui::Text("Active");
		ImGui::SameLine();
		ImGui::Checkbox("##meshActive", &mesh->active);

		ImGui::Text("Texture");
		ImGui::SameLine();
		ImGui::Checkbox("##textActive", &mesh->drawTexture);
		
		//ImGui::Text("Path: %s", mesh->path.c_str());
		//ImGui::Separator();
	
		//ImGui::Text("Mesh %d",i);
		ImGui::Text("Name: %s", mesh->GetMesh()->name.c_str());
		/*ImGui::Checkbox("Mesh AABB", &mesh->drawAABB);
		ImGui::Text("Draw textures");
		ImGui::SameLine();
		ImGui::Checkbox("##drawText", &mesh->drawTexture);*/
		ImGui::Text("Vertex normals");
		ImGui::SameLine();
		ImGui::Checkbox("##DrawVNormal", &mesh->drawVertexNormals);
		ImGui::Text("Face normals");
		ImGui::SameLine();
		ImGui::Checkbox("##DrawFNormal", &mesh->drawFaceNormals);
		ImGui::Text("Vertices: %d", mesh->GetMesh()->verticesSize);
		if(mesh->GetMaterial() != nullptr) ImGui::Text("Material: %s", mesh->GetMaterial()->name.c_str());
		else ImGui::Text("Material: -");
		ImGui::Separator();
	}
}

void InspectorWindow::MaterialSection(GameObject* go)
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		Material* material = go->GetFirstComponentType(MATERIAL_COMP)->AsMaterial();

		RES_Material* mat = material->GetMaterial();
		if (mat != nullptr)
		{
			bool colorChange = false;
			//ImGui::Text("Material %d", i);		
			ImGui::Text("Name: %s", mat->name.c_str());
			//ImGui::Text("Path: %s", mat->assetPath.c_str());

			//ImGui::Separator();

			ImGui::Text("Width: %d", mat->textWidth);
			//ImGui::SameLine();
			ImGui::Text("Height: %d", mat->textHeight);
			/*
			Color tempColor = mat->color;
			ImGui::Text("Color");
			ImGui::Text("R");
			ImGui::SameLine();
			ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
			if (ImGui::InputFloat("##ColorR", &tempColor.r, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) colorChange = true;
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::Text("G");
			ImGui::SameLine();
			ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
			if (ImGui::InputFloat("##ColorG", &tempColor.g, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) colorChange = true;
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::Text("B");
			ImGui::SameLine();
			ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
			if (ImGui::InputFloat("##ColorB", &tempColor.b, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) colorChange = true;
			ImGui::PopItemWidth();

			ImGui::Text("Alpha");
			ImGui::SameLine();
			ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.20f);
			if (ImGui::InputFloat("##ColorA", &tempColor.a, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) colorChange = true;
			ImGui::PopItemWidth();*/

			ImGui::Separator();

			//ImGui::Spacing();
			ImGui::Image((ImTextureID)mat->textureID, ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
			//if (colorChange) mat->ChangeColor(tempColor);
			//ImGui::Separator();
			//ImGui::Spacing();
		}
	}
}