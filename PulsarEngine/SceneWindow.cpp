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
	bool inScene = ImGui::IsWindowHovered();
	App->editor->mouse_in_scene = inScene;
	ImVec2 winSize = ImGui::GetWindowSize();

	ImGui::Indent((winSize.x/2) -(75 *2));
	if(ImGui::Button("Play",ImVec2(75.0f,25.0f))) App->scene->state = SCENE_PLAY;
	ImGui::SameLine();
	if (ImGui::Button("Pause", ImVec2(75.0f, 25.0f))) App->scene->state = SCENE_PAUSE;
	ImGui::SameLine();
	if (ImGui::Button("Stop", ImVec2(75.0f, 25.0f))) App->scene->state = SCENE_STOP;
	//ImGui::SameLine();
	//if (ImGui::Button("Step", ImVec2(75.0f, 25.0f))) App->scene->state = SCENE_STEP;
	ImGui::Unindent();	
		
	if (winSize.x != lastSizeX || winSize.y != lastSizeY) SetNewSize(winSize.x, winSize.y);	
	ImGui::SetCursorPos(ImVec2(offsetX, offsetY));	
	cornerX = ImGui::GetCursorScreenPos().x;
	cornerY = ImGui::GetCursorScreenPos().y + windowSizeY;
	cornerY = App->window->height - cornerY;
	ImGui::Image((ImTextureID)App->renderer3D->renderTexture, winSize);		

	//Handle guizmo
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) gizmoOperation = ImGuizmo::OPERATION::ROTATE;
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) gizmoOperation = ImGuizmo::OPERATION::SCALE;

	if (!App->editor->selectedGameObjects.empty()) HandleGuizmo();

	//Handle click
	if (ImGuizmo::IsUsing() == false)
	{
		if (inScene && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) HandleClick();
	}

	ImGui::End();

	App->renderer3D->RenderLine(nearClick,farClick);

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


float2 SceneWindow::SceneToWindow(float2 p) 
{
	float2 ret;
	ret.x = p.x - cornerX;
	ret.y = p.y - cornerY;
	ret.x = ret.x / windowSizeX * App->window->width;
	ret.y = ret.y / windowSizeY * App->window->height;
	return ret;
}


float2 SceneWindow::WindowToScene(float2 p) 
{
	float2 ret; 
	ret.x = (p.x / App->window->width * windowSizeX) + cornerX;
	ret.y = (p.y / App->window->height * windowSizeY) + cornerY;
	return ret;
}

void SceneWindow::HandleClick()
{
	vec mousePos;
	mousePos.x = App->input->GetMouseX();
	mousePos.y = App->input->GetMouseY();
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
	float tempCornerX = cornerX;
	float tempCornerY = App->window->height - cornerY - windowSizeY;
	ImGuizmo::SetRect(tempCornerX, tempCornerY,windowSizeX, windowSizeY);

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