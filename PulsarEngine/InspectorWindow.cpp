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
		std::vector<Component*> temp;
		temp = go->GetAllComponentsByType(BOX_COLLIDER_COMP);
		if (temp.size() > 0)
		{
			for (int i = 0; i < temp.size(); i++)
			{
				BoxColliderSection(temp[i]->AsBoxCollider(),i);
			}
		}
		temp = go->GetAllComponentsByType(SPHERE_COLLIDER_COMP);
		if (temp.size() > 0)
		{
			for (int i = 0; i < temp.size(); i++)
			{
				SphereColliderSection(temp[i]->AsSphereCollider(),i);
			}
		}

		ImVec2 winSize = ImGui::GetContentRegionAvail();
		ImGui::Spacing();
		ImGui::Separator();
		if (ImGui::Button("Add component")) ImGui::OpenPopup(go->name.c_str());

		if (ImGui::BeginPopup(go->name.c_str()))
		{
			if (ImGui::Button("Mesh"))
			{
				go->AddComponent(MESH_COMP);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Material"))
			{
				go->AddComponent(MATERIAL_COMP);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Camera"))
			{
				go->AddComponent(CAMERA_COMP);
				ImGui::CloseCurrentPopup();
			}

			/*if (ImGui::Button("Rigidbody"))
			{
				//go->AddComponent(CAMERA_COMP);
				ImGui::CloseCurrentPopup();
			}*/

			if (ImGui::Button("BoxCollider"))
			{
				go->AddComponent(BOX_COLLIDER_COMP);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("SphereCollider"))
			{
				go->AddComponent(SPHERE_COLLIDER_COMP);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
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
		if (mesh != nullptr)
		{
			ImGui::Text("Active");
			ImGui::SameLine();
			ImGui::Checkbox("##meshActive", &mesh->active);

			ImGui::Text("Texture");
			ImGui::SameLine();
			ImGui::Checkbox("##textActive", &mesh->drawTexture);

			if (mesh->GetMesh() != nullptr)
			{
				ImGui::Text("Name: %s", mesh->GetMesh()->name.c_str());
				ImGui::Text("Vertex normals");
				ImGui::SameLine();
				ImGui::Checkbox("##DrawVNormal", &mesh->drawVertexNormals);
				ImGui::Text("Face normals");
				ImGui::SameLine();
				ImGui::Checkbox("##DrawFNormal", &mesh->drawFaceNormals);
				ImGui::Text("Vertices: %d", mesh->GetMesh()->verticesSize);
				if (mesh->GetMaterial() != nullptr) ImGui::Text("Material: %s", mesh->GetMaterial()->name.c_str());
				else ImGui::Text("Material: -");				
			}
		}
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
		}
		ImGui::Separator();
		if (ImGui::Button("Delete component")) go->DeleteGOComponent(MATERIAL_COMP);
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

void InspectorWindow::BoxColliderSection(BoxCollider* coll,int index)
{
	if (ImGui::CollapsingHeader("BoxCollider", ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::string tagStatic = "##boxCollStatic";
		tagStatic.append(std::to_string(index));
		std::string tagPosx = "##boxcollPosX";
		tagPosx.append(std::to_string(index));
		std::string tagPosy = "##boxcollPosY";
		tagPosy.append(std::to_string(index));
		std::string tagPosz = "##boxcollPosZ";
		tagPosz.append(std::to_string(index));
		std::string tagScalex = "##boxcollSizeX";
		tagScalex.append(std::to_string(index));
		std::string tagScaley = "##boxcollSizeY";
		tagScaley.append(std::to_string(index));
		std::string tagScalez = "##boxcollSizeZ";
		tagScalez.append(std::to_string(index));
		bool collSt = coll->IsStatic();
		ImGui::Text("Static: ");
		ImGui::SameLine();
		if (ImGui::Checkbox(tagStatic.c_str(), &collSt)) coll->SetStatic(collSt);

		float3 pos = coll->GetPosition();
		float3 scale = coll->GetSize();
		bool changePos = false;
		bool changeScale = false;
		ImGui::Text("Position x:");
		ImGui::SameLine();
		if (ImGui::InputFloat(tagPosx.c_str(), &pos.x, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) changePos = true;
		ImGui::Text("Position y:");
		ImGui::SameLine();
		if (ImGui::InputFloat(tagPosy.c_str(), &pos.y, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) changePos = true;
		ImGui::Text("Position z:");
		ImGui::SameLine();
		if (ImGui::InputFloat(tagPosz.c_str(), &pos.z, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) changePos = true;

		ImGui::Text("Size x:");
		ImGui::SameLine();
		if (ImGui::InputFloat(tagScalex.c_str(), &scale.x, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) changeScale = true;
		ImGui::Text("Size y:");
		ImGui::SameLine();
		if (ImGui::InputFloat(tagScaley.c_str(), &scale.y, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) changeScale = true;
		ImGui::Text("Size z:");
		ImGui::SameLine();
		if (ImGui::InputFloat(tagScalez.c_str(), &scale.z, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) changeScale = true;

		if (changePos) coll->SetPos(pos);
		if (changeScale) coll->SetScale(scale);

		if (ImGui::Button("Delete component")) coll->gameobject->DeleteGOComponent(coll->UUID);
	}
}

void InspectorWindow::SphereColliderSection(SphereCollider* coll, int index)
{
	if (ImGui::CollapsingHeader("SphereCollider", ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::string tagStatic = "##sphereCollStatic";
		tagStatic.append(std::to_string(index));
		std::string tagPosx = "##spherecollPosX";
		tagPosx.append(std::to_string(index));
		std::string tagPosy = "##spherecollPosY";
		tagPosy.append(std::to_string(index));
		std::string tagPosz = "##spherecollPosZ";
		tagPosz.append(std::to_string(index));
		std::string tagScalex = "##spherecollSizeX";
		tagScalex.append(std::to_string(index));
		std::string tagScaley = "##spherecollSizeY";
		tagScaley.append(std::to_string(index));
		std::string tagScalez = "##spherecollSizeZ";
		tagScalez.append(std::to_string(index));
		bool collSt = coll->IsStatic();
		ImGui::Text("Static: ");
		ImGui::SameLine();
		if (ImGui::Checkbox(tagStatic.c_str(), &collSt)) coll->SetStatic(collSt);

		float3 pos = coll->GetPosition();
		float3 scale = coll->GetSize();
		bool changePos = false;
		bool changeScale = false;
		ImGui::Text("Position x:");
		ImGui::SameLine();
		if (ImGui::InputFloat(tagPosx.c_str(), &pos.x, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) changePos = true;
		ImGui::Text("Position y:");
		ImGui::SameLine();
		if (ImGui::InputFloat(tagPosy.c_str(), &pos.y, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) changePos = true;
		ImGui::Text("Position z:");
		ImGui::SameLine();
		if (ImGui::InputFloat(tagPosz.c_str(), &pos.z, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) changePos = true;

		ImGui::Text("Size x:");
		ImGui::SameLine();
		if (ImGui::InputFloat(tagScalex.c_str(), &scale.x, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) changeScale = true;
		ImGui::Text("Size y:");
		ImGui::SameLine();
		if (ImGui::InputFloat(tagScaley.c_str(), &scale.y, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) changeScale = true;
		ImGui::Text("Size z:");
		ImGui::SameLine();
		if (ImGui::InputFloat(tagScalez.c_str(), &scale.z, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) changeScale = true;

		if (changePos) coll->SetPos(pos);
		if (changeScale) coll->SetScale(scale);

		if (ImGui::Button("Delete component")) coll->gameobject->DeleteGOComponent(coll->UUID);
	}
}