#include "Globals.h"
#include "Application.h"
#include "SceneWindow.h"
#include "Transform.h"
#include "Mesh.h"
#include "RES_Mesh.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "ImGuizmo/ImGuizmo.h"

#include <string>

SceneWindow::SceneWindow(std::string name) : EditorWindow(name)
{
	flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize;
	corners.x = 0;
	corners.y = 0;
	windowSize.x = 0;
	windowSize.y = 0;
	offsets.x = 0;
	offsets.y = 0;
}

SceneWindow::~SceneWindow()
{
	LOG("Delete scene");
}


update_status SceneWindow::Draw()
{
	update_status ret = UPDATE_CONTINUE;
    ImGui::Begin(name.c_str(), &active,flags);
	App->editor->mouse_in_scene = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
	ImVec2 winSize = ImGui::GetContentRegionAvail();

	ImGui::Indent((winSize.x/2) -(75 *2));
	if(ImGui::Button("Play",ImVec2(75.0f,25.0f))) App->scene->state = SCENE_PLAY;
	ImGui::SameLine();
	if (ImGui::Button("Pause", ImVec2(75.0f, 25.0f))) App->scene->state = SCENE_PAUSE;
	ImGui::SameLine();
	if (ImGui::Button("Stop", ImVec2(75.0f, 25.0f))) App->scene->state = SCENE_STOP;
	//ImGui::SameLine();
	//if (ImGui::Button("Step", ImVec2(75.0f, 25.0f))) App->scene->state = SCENE_STEP;
	ImGui::Unindent();	
		
	ImGui::SetCursorPos(ImVec2(offsets.x, offsets.y));
	corners.x = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x;
	corners.y = ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y;
	if (windowSize.x != winSize.x || windowSize.y != winSize.y) SetNewSize(winSize.x, winSize.y);
	
	ImGui::Image((ImTextureID)App->renderer3D->renderTexture, winSize);		

	//Handle guizmo
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) gizmoOperation = ImGuizmo::OPERATION::ROTATE;
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) gizmoOperation = ImGuizmo::OPERATION::SCALE;

	if (!App->editor->selectedGameObjects.empty()) HandleGuizmo();

	//Handle click
	if (ImGuizmo::IsUsing() == false)
	{
		if (App->editor->mouse_in_scene && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) HandleClick();
	}

	ImGui::End();

	App->renderer3D->RenderLine(nearClick,farClick);

	return ret;
}

void SceneWindow::SetNewSize(float x, float y)
{
	windowSize.x = x;
	windowSize.y = y;

	App->renderer3D->OnResize(windowSize.x, -windowSize.y);
	//App->camera->ResizeView(windowSize.x, windowSize.y);
}


float2 SceneWindow::SceneToWindow(float2 p) 
{
	float2 ret;
	ret.x = p.x - corners.x;
	ret.y = p.y - corners.y;
	ret.x = ret.x / windowSize.x * App->window->width;
	ret.y = ret.y / windowSize.y * App->window->height;
	return ret;
}


float2 SceneWindow::WindowToScene(float2 p) 
{
	float2 ret; 
	ret.x = (p.x / App->window->width * windowSize.x) + corners.x;
	ret.y = (p.y / App->window->height * windowSize.y) + corners.y;
	return ret;
}

void SceneWindow::HandleClick()
{
	mousePos.x = App->input->GetMouseX() - corners.x;;
	mousePos.y = App->input->GetMouseY() - corners.y;
	float mouseNX = mousePos.x / (float)App->window->width;
	float mouseNY = -mousePos.y / (float)App->window->height;
	//float mouseNX = mousePos.x / (float)winSize.x;
	//float mouseNY = mousePos.y / (float)winSize.y;

	mouseNX = (mouseNX - 0.5) / 0.5;
	mouseNY = (mouseNY - 0.5) / 0.5;
	std::vector<GameObject*> gameobjects = App->camera->GetDrawnObjects();
	LineSegment ray = App->camera->CastRay(mouseNX, mouseNY);
	nearClick = ray.a;
	farClick = ray.b;
	std::vector<GameObject*> intersections;
	for (std::vector<GameObject*>::iterator it = gameobjects.begin(); it != gameobjects.end(); it++)
	{
		GameObject* temp = (*it)->CheckRayIntersect(ray);
		if (temp != nullptr) intersections.push_back((*it));
	}
	
	bool gotGo = false;
	LOG("Intersections size = %d", intersections.size());
	if (!intersections.empty())
	{
		for (std::vector<GameObject*>::iterator it = intersections.begin(); it != intersections.end(); it++)
		{
			Mesh* mesh = (*it)->GetFirstComponentType(MESH_COMP)->AsMesh();
			LineSegment temp = ray;
			temp.Transform((*it)->transform->GetGlobalTransform().Inverted());
			int bufferSize = mesh->GetMesh()->indexSize;
			uint* buffer = mesh->GetMesh()->indicesArray;
			float* vertices = mesh->GetMesh()->verticesArray;
			for (int i = 0; i < bufferSize; i += 3)
			{
				//LOG("Triangle: %d",i);
				uint a = buffer[i] * 3;
				vec v1(vertices[a]);

				uint b = buffer[i + 1] * 3;
				vec v2(vertices[b]);

				uint c = buffer[i + 2] * 3;
				vec v3(vertices[c]);

				Triangle t(v1, v2, v3);
				if (temp.Intersects(t, nullptr, nullptr))
				{
					LOG("Gameobject %s selected", (*it)->name.c_str());
					App->editor->SelectOne((*it));
					gotGo = true;
					break;
				}
			}
			if (gotGo) break;
		}
		if (!gotGo) App->editor->EmptySelected();
	}
	else
	{
		App->editor->EmptySelected();
	}
}

void SceneWindow::HandleGuizmo()
{
	GameObject* gameObject = App->editor->selectedGameObjects[0];

	float4x4 viewMatrix = App->camera->camera->frustum.ViewMatrix();
	viewMatrix.Transpose();
	float4x4 projectionMatrix = App->camera->camera->frustum.ProjectionMatrix().Transposed();
	float4x4 modelProjection = gameObject->transform->GetGlobalTransformTransposed();
	float4x4 modelChangeMat = gameObject->transform->GetTransformTransposed();

	ImGuizmo::SetDrawlist();
	//float tempCornerX = corners.x;
	//float tempCornerY = App->window->height - cornerY - windowSizeY;
	ImGuizmo::SetRect(corners.x, corners.y,windowSize.x, windowSize.y);

	float modelPtr[16];
	memcpy(modelPtr, modelChangeMat.ptr(), 16 * sizeof(float));
	//ImGuizmo::MODE finalMode; 
	//if (gizmoOperation == ImGuizmo::OPERATION::SCALE) finalMode = ImGuizmo::MODE::LOCAL;
	//else finalMode = gizmoMode;
	ImGuizmo::Manipulate(viewMatrix.ptr(), projectionMatrix.ptr(), gizmoOperation, gizmoMode, modelPtr,modelProjection.ptr());

	if (ImGuizmo::IsUsing())
	{
		float4x4 newMatrix;
		newMatrix.Set(modelPtr);
		gameObject->transform->SetLocalTransform(newMatrix.Transposed());
	}
}