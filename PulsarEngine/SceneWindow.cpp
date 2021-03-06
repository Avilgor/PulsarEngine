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
}

SceneWindow::~SceneWindow()
{
	LOG("Delete scene");
}


update_status SceneWindow::Draw()
{
	update_status ret = UPDATE_CONTINUE;
	ImGui::Begin(name.c_str(), &active, flags);
	App->editor->mouse_in_scene = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
	ImVec2 winSize = ImGui::GetWindowSize();

	ImGui::Indent((winSize.x / 2) - (75 * 2));
	bool changeColor = false;
	
	if (App->scene->GetSceneState() == SCENE_RUNNING) changeColor = true; 
	if(changeColor) ImGui::PushStyleColor(ImGuiCol_Button, pressColor);
	if (ImGui::Button("Play", ImVec2(75.0f, 25.0f))) App->scene->state = SCENE_PLAY;
	if (changeColor) ImGui::PopStyleColor();
	changeColor = false;

	ImGui::SameLine();
	if (App->scene->GetSceneState() == SCENE_PAUSE) changeColor = true;
	if (changeColor) ImGui::PushStyleColor(ImGuiCol_Button, pressColor);
	if (ImGui::Button("Pause", ImVec2(75.0f, 25.0f)))
	{
		if (App->scene->GetSceneState() == SCENE_RUNNING) App->scene->state = SCENE_PAUSE;
		else if (App->scene->GetSceneState() == SCENE_PAUSE) App->scene->state = SCENE_UNPAUSE;
	}
	if (changeColor) ImGui::PopStyleColor();
	changeColor = false;

	ImGui::SameLine();
	if (App->scene->GetSceneState() == SCENE_STOP) changeColor = true;
	if (changeColor) ImGui::PushStyleColor(ImGuiCol_Button, pressColor);
	if (ImGui::Button("Stop", ImVec2(75.0f, 25.0f))) App->scene->state = SCENE_STOP;
	if (changeColor) ImGui::PopStyleColor();

	ImGui::Unindent();
	ImGui::SetCursorPos(ImVec2(0, -10));
	corners.x = ImGui::GetWindowPos().x;
	corners.y = ImGui::GetWindowPos().y;
	if (windowSize.x != winSize.x || windowSize.y != winSize.y) SetNewSize(winSize.x, winSize.y);

	ImGui::Image((ImTextureID)App->renderer3D->renderTexture, winSize);

	//Handle guizmo
	if (ImGui::IsWindowHovered())
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) gizmoOperation = ImGuizmo::OPERATION::ROTATE;
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) gizmoOperation = ImGuizmo::OPERATION::SCALE;
	}
	if (!App->editor->selectedGameObjects.empty()) HandleGuizmo();

	//Handle click
	if (ImGuizmo::IsUsing() == false)
	{
		if (App->editor->mouse_in_scene && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) HandleClick();
	}

	ImGui::End();

	//App->renderer3D->RenderLine(nearClick,farClick);

	return ret;
}

void SceneWindow::SetNewSize(float x, float y)
{
	windowSize.x = x;
	windowSize.y = y;

	App->renderer3D->OnResize(windowSize.x, -windowSize.y);
}


void SceneWindow::HandleClick()
{
	float halfwidth = App->window->width / 2;
	float halfheight = App->window->height / 2;
	mousePos.x = App->input->GetMouseX();
	mousePos.y = App->window->height - App->input->GetMouseY();
	float mouseNY = 0;
	float mouseNX = 0;
	if (mousePos.x >= halfwidth) //+
	{
		mousePos.x -= halfwidth;
		mouseNX = mousePos.x / halfwidth;	
	}
	else //-
	{
		mouseNX = -1 + (mousePos.x / halfwidth);
	}

	if (mousePos.y >= halfheight) //+
	{
		mousePos.y -= halfheight;
		mouseNY = mousePos.y / halfheight;
	}
	else //-
	{
		mouseNY = -1 + (mousePos.y / halfheight);
	}	

	std::vector<GameObject*> gameobjects = App->camera->GetDrawnObjects();	
	LineSegment ray = App->camera->CastRay(mouseNX, mouseNY);	
	//nearClick = ray.a;
	//farClick = ray.b;
	//Check AABB intersections
	std::vector<GameObject*> intersections;
	for (std::vector<GameObject*>::iterator it = gameobjects.begin(); it != gameobjects.end(); it++)
	{
		GameObject* temp = (*it)->CheckRayIntersect(ray);
		if (temp != nullptr) intersections.push_back((*it));
	}	

	//Check mesh intersections
	if (!intersections.empty())
	{
		float minDist = 999999;
		GameObject* first = nullptr;
		for (std::vector<GameObject*>::iterator it = intersections.begin(); it != intersections.end(); it++)
		{
			Component* comp = (*it)->GetFirstComponentType(MESH_COMP);
			if (comp != nullptr)
			{
				Mesh* mesh = comp->AsMesh();
				if (mesh != nullptr)
				{
					LineSegment temp = ray;
					temp.Transform((*it)->transform->GetGlobalTransform().Inverted());
					if (mesh->GetMesh() != nullptr)
					{
						int bufferSize = mesh->GetMesh()->indexSize;
						uint* buffer = mesh->GetMesh()->indicesArray;
						float* vertices = mesh->GetMesh()->verticesArray;
						int numVertices = mesh->GetMesh()->verticesSize * 3;

						for (uint v = 0; v < bufferSize - 3; v += 3)
						{
							uint indexA = buffer[v] * 3;
							if (indexA < numVertices)
							{
								vec t1(&vertices[indexA]);

								uint indexB = buffer[v + 1] * 3;
								if (indexB < numVertices)
								{
									vec t2(&vertices[indexB]);

									uint indexC = buffer[v + 2] * 3;
									if (indexC < numVertices)
									{
										vec t3(&vertices[indexC]);

										Triangle triangle(t1, t2, t3);

										float distance;
										float3 intersectionPoint;
										if (temp.Intersects(triangle, &distance, &intersectionPoint))
										{
											if (distance < minDist)
											{
												//LOG("New distance %f", distance);
												minDist = distance;
												first = (*it);
											}
											break;
										}
									}
								}
						
							}
						}
					}
				}
			}
		}

		if (first != nullptr)
		{
			App->editor->SelectOne(first);
		}
		else App->editor->EmptySelected();
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
	float4x4 globalMat = gameObject->transform->GetGlobalTransformTransposed();

	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(0, 0,App->window->width, App->window->height);

	ImGuizmo::Manipulate(viewMatrix.ptr(), projectionMatrix.ptr(), gizmoOperation, gizmoMode, globalMat.ptr());

	if (ImGuizmo::IsUsing())
	{
		gameObject->transform->SetGlobalTransform(globalMat.Transposed());
		gameObject->SetTransformUpdate(true);
	}
}