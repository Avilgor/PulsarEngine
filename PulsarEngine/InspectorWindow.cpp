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
#include "CapsuleCollider.h"
#include "ConstraintPoint.h"
#include "ConstraintHinge.h"
#include "ConstraintSlider.h"
#include "ConstraintCone.h"
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
		currentGo = App->editor->selectedGameObjects[0];
		GameObjectSection(currentGo);
		if (currentGo->transform != nullptr) TransformSection(currentGo);
		if (currentGo->GetFirstComponentType(MESH_COMP)) MeshSection(currentGo);
		if (currentGo->GetFirstComponentType(MATERIAL_COMP)) MaterialSection(currentGo);
		if (currentGo->camera != nullptr) CameraSection(currentGo);
		if (currentGo->boxcollider != nullptr) BoxColliderSection(currentGo->boxcollider);
		if (currentGo->spherecollider != nullptr) SphereColliderSection(currentGo->spherecollider);
		if (currentGo->capsulecollider != nullptr) CapsuleColliderSection(currentGo->capsulecollider);
		if (currentGo->pointconstraint != nullptr) PointConstraintSection(currentGo->pointconstraint);
		if (currentGo->hingeconstraint != nullptr) HingeConstraintSection(currentGo->hingeconstraint);
		if (currentGo->sliderconstraint != nullptr) SliderConstraintSection(currentGo->sliderconstraint);
		if (currentGo->coneconstraint != nullptr) ConeConstraintSection(currentGo->coneconstraint);

		ImVec2 winSize = ImGui::GetContentRegionAvail();
		ImGui::Spacing();
		ImGui::Separator();
		if (ImGui::Button("Add component")) ImGui::OpenPopup(currentGo->name.c_str());

		if (ImGui::BeginPopup(currentGo->name.c_str()))
		{
			if (ImGui::Button("Mesh"))
			{
				currentGo->AddComponent(MESH_COMP);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Material"))
			{
				currentGo->AddComponent(MATERIAL_COMP);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Camera"))
			{
				currentGo->AddComponent(CAMERA_COMP);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Box Collider"))
			{
				currentGo->AddComponent(BOX_COLLIDER_COMP);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Sphere Collider"))
			{
				currentGo->AddComponent(SPHERE_COLLIDER_COMP);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Capsule Collider"))
			{
				currentGo->AddComponent(CAPSULE_COLLIDER_COMP);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Point constraint"))
			{
				currentGo->AddComponent(CONSTRAINT_POINT_COMP);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Hinge constraint"))
			{
				currentGo->AddComponent(CONSTRAINT_HINGE_COMP);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Slider constraint"))
			{
				currentGo->AddComponent(CONSTRAINT_SLIDER_COMP);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Cone constraint"))
			{
				currentGo->AddComponent(CONSTRAINT_CONE_COMP);
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
		if (ImGui::Button("Delete component##delmesh")) go->DeleteGOComponent(MESH_COMP);
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
	
			ImGui::Text("Name: %s", mat->name.c_str());
			ImGui::Text("Width: %d", mat->textWidth);
			ImGui::Text("Height: %d", mat->textHeight);

			ImGui::Separator();
			ImGui::Image((ImTextureID)mat->textureID, ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::Separator();
		}
		ImGui::Separator();
		if (ImGui::Button("Delete component##delmaterial")) go->DeleteGOComponent(MATERIAL_COMP);
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
		if (ImGui::Button("Delete component##delcamera")) go->DeleteGOComponent(CAMERA_COMP);
	}
}

void InspectorWindow::BoxColliderSection(BoxCollider* coll)
{
	if (ImGui::CollapsingHeader("BoxCollider", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool collSt = coll->IsStatic();
		ImGui::Text("Static:");
		ImGui::SameLine();
		if (ImGui::Checkbox("##boxCollStatic", &collSt)) coll->SetStatic(collSt);

		bool collTrigger = coll->isTrigger;
		ImGui::Text("Trigger:");
		ImGui::SameLine();
		if (ImGui::Checkbox("##boxCollTrigger", &collTrigger)) coll->SetTrigger(collTrigger);

		ImGui::Text("Draw:");
		ImGui::SameLine();
		ImGui::Checkbox("##boxCollDraw", &coll->draw);

		float mass = coll->mass;
		ImGui::Text("Mass:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##boxCollMass", &mass, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) coll->SetMass(mass);

		float friction = coll->friction;
		ImGui::Text("Friction:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##boxCollFriction", &friction, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) coll->SetFriction(friction);	

		float3 pos = coll->GetPosition();
		float3 scale = coll->GetSize();
		bool changePos = false;
		bool changeScale = false;
		ImGui::Text("Position x:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##boxcollPosX", &pos.x, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) changePos = true;
		ImGui::Text("Position y:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##boxcollPosY", &pos.y, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) changePos = true;
		ImGui::Text("Position z:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##boxcollPosZ", &pos.z, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) changePos = true;

		ImGui::Text("Size x:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##boxcollSizeX", &scale.x, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) changeScale = true;
		ImGui::Text("Size y:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##boxcollSizeY", &scale.y, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) changeScale = true;
		ImGui::Text("Size z:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##boxcollSizeZ", &scale.z, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) changeScale = true;

		if (changePos) coll->SetPos(pos);
		if (changeScale) coll->SetScale(scale);

		if (ImGui::Button("Delete Component##delboxcoll")) currentGo->DeleteGOComponent(BOX_COLLIDER_COMP);		
	}
}

void InspectorWindow::SphereColliderSection(SphereCollider* coll)
{
	if (ImGui::CollapsingHeader("SphereCollider", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool collSt = coll->IsStatic();
		ImGui::Text("Static: ");
		ImGui::SameLine();
		if (ImGui::Checkbox("##sphereCollStatic", &collSt)) coll->SetStatic(collSt);

		bool collTrigger = coll->isTrigger;
		ImGui::Text("Trigger:");
		ImGui::SameLine();
		if (ImGui::Checkbox("##sphereCollTrigger", &collTrigger)) coll->SetTrigger(collTrigger);

		ImGui::Text("Draw:");
		ImGui::SameLine();
		ImGui::Checkbox("##sphereCollDraw", &coll->draw);

		float mass = coll->mass;
		ImGui::Text("Mass:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##sphereCollMass", &mass, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) coll->SetMass(mass);

		float friction = coll->friction;
		ImGui::Text("Friction:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##sphereCollFriction", &friction, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) coll->SetFriction(friction);

		float3 pos = coll->GetPosition();
		float rad = coll->GetSize().x;
		bool changePos = false;
		ImGui::Text("Position x:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##spherecollPosX", &pos.x, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) changePos = true;
		ImGui::Text("Position y:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##spherecollPosY", &pos.y, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) changePos = true;
		ImGui::Text("Position z:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##spherecollPosZ", &pos.z, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) changePos = true;

		ImGui::Text("Radius:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##spherecollRadius", &rad, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) coll->SetScale(rad);

		if (changePos) coll->SetPos(pos);

		if (ImGui::Button("Delete Component##delspherecoll")) currentGo->DeleteGOComponent(SPHERE_COLLIDER_COMP);
	}
}

void InspectorWindow::CapsuleColliderSection(CapsuleCollider* coll)
{
	if (ImGui::CollapsingHeader("CapsuleCollider", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool collSt = coll->IsStatic();
		ImGui::Text("Static: ");
		ImGui::SameLine();
		if (ImGui::Checkbox("##capsuleCollStatic", &collSt)) coll->SetStatic(collSt);

		bool collTrigger = coll->isTrigger;
		ImGui::Text("Trigger:");
		ImGui::SameLine();
		if (ImGui::Checkbox("##capsuleCollTrigger", &collTrigger)) coll->SetTrigger(collTrigger);

		ImGui::Text("Draw:");
		ImGui::SameLine();
		ImGui::Checkbox("##capsuleCollDraw", &coll->draw);

		float mass = coll->mass;
		ImGui::Text("Mass:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##capsuleCollMass", &mass, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) coll->SetMass(mass);

		float friction = coll->friction;
		ImGui::Text("Friction:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##capsuleCollFriction", &friction, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) coll->SetFriction(friction);

		float3 pos = coll->GetPosition();
		float rad = coll->GetSize().x;
		float height = coll->GetSize().y;
		
		bool changePos = false;
		ImGui::Text("Position x:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##capsulecollPosX", &pos.x, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) changePos = true;
		ImGui::Text("Position y:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##capsulecollPosY", &pos.y, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) changePos = true;
		ImGui::Text("Position z:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##capsulecollPosZ", &pos.z, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) changePos = true;

		bool resize = false;
		ImGui::Text("Radius:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##capsulecollRadius", &rad, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) resize = true;
		ImGui::Text("Height:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##capsulecollHeight", &height, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) resize = true;

		if (changePos) coll->SetPos(pos);
		if (resize)
		{
			if (rad > 0 && height > 0) coll->SetScale(rad, height);
		}

		if (ImGui::Button("Delete Componen##delcapsulecoll")) currentGo->DeleteGOComponent(CAPSULE_COLLIDER_COMP);	
	}
}

void InspectorWindow::PointConstraintSection(ConstraintPoint* point)
{
	if (ImGui::CollapsingHeader("Point constraint", ImGuiTreeNodeFlags_DefaultOpen))
	{
		char name[50];
		if (point->bodyB != nullptr)
		{
			if (point->bodyBComp != nullptr) strcpy_s(name, 50, point->bodyBComp->gameobject->name.c_str());
		}
		else strcpy_s(name, 50, " ");
		ImGui::Text("Body B");
		ImGui::InputText("##bodyBpoint", name, 50, ImGuiInputTextFlags_ReadOnly);
		if (App->editor->leftMouse == KEY_UP)
		{
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
			{
				if (App->editor->dragObject != nullptr)
				{
					if (App->editor->dragObject->HasCollider())
					{
						point->SetBodyB(App->editor->dragObject->GetColliderComp());
					}
				}
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear##clearpointconst"))
		{
			point->ClearBodyB();
		}
		ImGui::Separator();

		bool changePos = false;
		float3 offA = point->GetOffsetA();
		ImGui::Text("Local Offset X:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##localOffsetXpoint", &offA.x, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) changePos = true;
		ImGui::Text("Local Offset Y:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##localOffsetYpoint", &offA.y, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) changePos = true;
		ImGui::Text("Local Offset Z:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##localOffsetZpoint", &offA.z, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) changePos = true;
		if (changePos) point->SetOffsetA(offA);

		changePos = false;
		float3 offB = point->GetOffsetB();
		ImGui::Text("Body Offset X:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##bodyOffsetXpoint", &offB.x, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) changePos = true;
		ImGui::Text("Body Offset Y:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##bodyOffsetYpoint", &offB.y, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) changePos = true;
		ImGui::Text("Body Offset Z:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##bodyOffsetZpoint", &offB.z, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) changePos = true;
		if (changePos) point->SetOffsetB(offB);

		if (ImGui::Button("Delete Component##delpointconst")) currentGo->DeleteGOComponent(CONSTRAINT_POINT_COMP);
	}
}

void InspectorWindow::HingeConstraintSection(ConstraintHinge* hinge)
{
	if (ImGui::CollapsingHeader("Hinge constraint", ImGuiTreeNodeFlags_DefaultOpen))
	{
		char name[50];
		if (hinge->bodyB != nullptr)
		{
			if (hinge->bodyBComp != nullptr) strcpy_s(name, 50, hinge->bodyBComp->gameobject->name.c_str());
		}
		else strcpy_s(name, 50, " ");
		ImGui::Text("Body B");
		ImGui::InputText("##bodyBhinge", name, 50, ImGuiInputTextFlags_ReadOnly);
		if (App->editor->leftMouse == KEY_UP)
		{
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
			{
				if (App->editor->dragObject != nullptr)
				{
					if (App->editor->dragObject->HasCollider())
					{
						hinge->SetBodyB(App->editor->dragObject->GetColliderComp());
					}
				}
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear##clearhingeconst"))
		{
			hinge->ClearBodyB();
		}
		ImGui::Separator();

		bool change = false;
		float3 offA = hinge->GetOffsetA();
		ImGui::Text("Local Offset X:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##localOffsetXhinge", &offA.x, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		ImGui::Text("Local Offset Y:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##localOffsetYhinge", &offA.y, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		ImGui::Text("Local Offset Z:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##localOffsetZhinge", &offA.z, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		if (change) hinge->SetOffsetA(offA);

		change = false;
		float3 offB = hinge->GetOffsetB();
		ImGui::Text("Body Offset X:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##bodyOffsetXhinge", &offB.x, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		ImGui::Text("Body Offset Y:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##bodyOffsetYhinge", &offB.y, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		ImGui::Text("Body Offset Z:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##bodyOffsetZhinge", &offB.z, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		if (change) hinge->SetOffsetB(offB);

		change = false;
		float3 axisA = hinge->GetAxisA();
		ImGui::Text("Axis A-X:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##axisAXhinge", &axisA.x, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		ImGui::Text("Axis A-Y:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##axisAYhinge", &axisA.y, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		ImGui::Text("Axis A-Z:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##axisAZhinge", &axisA.z, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		if (change) hinge->SetAxisA(axisA);

		change = false;
		float3 axisB = hinge->GetAxisB();
		ImGui::Text("Axis B-X:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##axisBXhinge", &axisB.x, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		ImGui::Text("Axis B-Y:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##axisBYhinge", &axisB.y, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		ImGui::Text("Axis B-Z:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##axisBZhinge", &axisB.z, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		if (change) hinge->SetAxisB(axisB);

		if (ImGui::Button("Delete Component##delhingeconst")) currentGo->DeleteGOComponent(CONSTRAINT_HINGE_COMP);
	}
}

void InspectorWindow::SliderConstraintSection(ConstraintSlider* slider)
{
	if (ImGui::CollapsingHeader("Slider constraint", ImGuiTreeNodeFlags_DefaultOpen))
	{
		char name[50];
		if (slider->bodyB != nullptr)
		{
			if (slider->bodyBComp != nullptr) strcpy_s(name, 50, slider->bodyBComp->gameobject->name.c_str());
		}
		else strcpy_s(name, 50, " ");
		ImGui::Text("Body B");
		ImGui::InputText("##bodyBslider", name, 50, ImGuiInputTextFlags_ReadOnly);
		if (App->editor->leftMouse == KEY_UP)
		{
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
			{
				if (App->editor->dragObject != nullptr)
				{
					if (App->editor->dragObject->HasCollider())
					{
						slider->SetBodyB(App->editor->dragObject->GetColliderComp());
					}
				}
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear##clearsliderconst"))
		{
			slider->ClearBodyB();
		}
		ImGui::Separator();

		/*bool change = false;
		float3 offA = slider->GetOffsetA();
		ImGui::Text("Local Offset X:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##localOffsetXslider", &offA.x, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		ImGui::Text("Local Offset Y:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##localOffsetYslider", &offA.y, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		ImGui::Text("Local Offset Z:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##localOffsetZslider", &offA.z, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		if (change) slider->SetOffsetA(offA);

		change = false;
		float3 offB = slider->GetOffsetB();
		ImGui::Text("Body Offset X:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##bodyOffsetXslider", &offB.x, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		ImGui::Text("Body Offset Y:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##bodyOffsetYslider", &offB.y, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		ImGui::Text("Body Offset Z:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##bodyOffsetZslider", &offB.z, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		if (change) slider->SetOffsetB(offB);*/

		if (ImGui::Button("Delete Component##delsliderconst")) currentGo->DeleteGOComponent(CONSTRAINT_SLIDER_COMP);
	}
}

void InspectorWindow::ConeConstraintSection(ConstraintCone* cone)
{
	if (ImGui::CollapsingHeader("Cone constraint", ImGuiTreeNodeFlags_DefaultOpen))
	{
		char name[50];
		if (cone->bodyB != nullptr)
		{
			if (cone->bodyBComp != nullptr) strcpy_s(name, 50, cone->bodyBComp->gameobject->name.c_str());
		}
		else strcpy_s(name, 50, " ");
		ImGui::Text("Body B");
		ImGui::InputText("##bodyBcone", name, 50, ImGuiInputTextFlags_ReadOnly);
		if (App->editor->leftMouse == KEY_UP)
		{
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
			{
				if (App->editor->dragObject != nullptr)
				{
					if (App->editor->dragObject->HasCollider())
					{
						cone->SetBodyB(App->editor->dragObject->GetColliderComp());
					}
				}
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear##clearconeconst"))
		{
			cone->ClearBodyB();
		}
		ImGui::Separator();

		/*bool change = false;
		float3 offA = cone->GetOffsetA();
		ImGui::Text("Local Offset X:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##localOffsetXcone", &offA.x, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		ImGui::Text("Local Offset Y:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##localOffsetYcone", &offA.y, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		ImGui::Text("Local Offset Z:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##localOffsetZcone", &offA.z, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		if (change) cone->SetOffsetA(offA);

		change = false;
		float3 offB = cone->GetOffsetB();
		ImGui::Text("Body Offset X:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##bodyOffsetXcone", &offB.x, 0, 0, 2, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		ImGui::Text("Body Offset Y:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##bodyOffsetYcone", &offB.y, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		ImGui::Text("Body Offset Z:");
		ImGui::SameLine();
		if (ImGui::InputFloat("##bodyOffsetZcone", &offB.z, 0, 0, 3, ImGuiInputTextFlags_EnterReturnsTrue)) change = true;
		if (change) cone->SetOffsetB(offB);*/

		if (ImGui::Button("Delete Component##delconeconst")) currentGo->DeleteGOComponent(CONSTRAINT_CONE_COMP);
	}
}