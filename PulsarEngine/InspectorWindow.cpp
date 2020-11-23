#include "Globals.h"
#include "Application.h"
#include "InspectorWindow.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "RES_Mesh.h"
#include "RES_Material.h"
#include "Camera.h"
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
		if (go->camera != nullptr) CameraSection(go);

		ImVec2 winSize = ImGui::GetContentRegionAvail();
		if (ImGui::Button("Add component"))
		{
			if (ImGui::BeginPopup(go->name.c_str()))
			{
				if (ImGui::Button("Mesh"))
				{
					go->AddComponent(MESH_COMP);
					//ImGui::CloseCurrentPopup();
				}

				if (ImGui::Button("Material"))
				{
					go->AddComponent(MATERIAL_COMP);
					//ImGui::CloseCurrentPopup();
				}

				if (ImGui::Button("Camera"))
				{
					go->AddComponent(CAMERA_COMP);
					//ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
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
		if (ImGui::Button("Delete component")) go->DeleteGOComponent(MESH_COMP);
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

			ImGui::Separator();
			ImGui::Image((ImTextureID)mat->textureID, ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::Separator();
			if (ImGui::Button("Delete component")) go->DeleteGOComponent(MATERIAL_COMP);
		}
	}	
}

void InspectorWindow::CameraSection(GameObject* go)
{
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		Camera* cam = go->camera;
		ImGui::Text("Main camera");
		ImGui::SameLine();
		ImGui::Checkbox("##cameraMain", &cam->mainCamera);
		if (cam->mainCamera) App->camera->SetMainCamera(cam);
		float fplane = cam->GetFarPlane();
		float nplane = cam->GetNearPlane();
		float fov = cam->GetFOV();
		float aspect = cam->GetAspectRatio();
		ImGui::Text("Near plane");
		if (ImGui::InputFloat("##camfplane", &nplane, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) cam->SetFarPlane(nplane);
		ImGui::Text("Far plane");
		if (ImGui::InputFloat("##camnplane", &fplane, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) cam->SetNearPlane(fplane);
		ImGui::Text("FOV");
		if (ImGui::InputFloat("##camfov", &fov, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) cam->SetFOV(fov);
		ImGui::Text("Aspect ratio");
		if (ImGui::InputFloat("##camaspect", &aspect, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) cam->SetAspectRatio(aspect);		
		ImGui::Separator();
		if (ImGui::Button("Delete component")) go->DeleteGOComponent(CAMERA_COMP);
	}
}