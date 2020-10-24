#include "Globals.h"
#include "Application.h"
#include "HierarchyWindow.h"
#include "GameObject.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include <string>

HierarchyWindow::HierarchyWindow(std::string name) : EditorWindow(name)
{}

HierarchyWindow::~HierarchyWindow()
{}


update_status HierarchyWindow::Draw()
{
	update_status ret = UPDATE_CONTINUE;
	SaveInputs();
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::Begin(name.c_str(), &active);
	DrawSceneGameObjects();

	ImGui::End();

	return ret;
}

void HierarchyWindow::SaveInputs()
{
	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RCTRL)) ctrl = true;
	else ctrl = false;

	leftMouse = App->input->GetMouseButton(SDL_BUTTON_LEFT);
	rightMouse = App->input->GetMouseButton(SDL_BUTTON_RIGHT);
}

void HierarchyWindow::DrawSceneGameObjects()
{
	std::vector<GameObject*> sceneObjects = App->scene->GetRoot()->GetAllChilds();
	for (std::vector<GameObject*>::iterator it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
	{
		DrawGameObject((*it));
	}
}

void HierarchyWindow::DrawGameObject(GameObject* go)
{
	ImGuiTreeNodeFlags nodeFlag = ImGuiTreeNodeFlags_OpenOnArrow;
	if (go->HasChilds() == false)
	{
		nodeFlag |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	if (go->active == false) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 0.4));

	if (go->selected)
	{
		nodeFlag |= ImGuiTreeNodeFlags_Selected;
		//ImGui::SetNextTreeNodeOpen(true);
	}
	
	go->showHierarchy = ImGui::TreeNodeEx(go->name.c_str(), nodeFlag);

	if (go->active == false) ImGui::PopStyleColor();

	NodeInput(go);

	/*if (ImGui::BeginPopup(go->name.c_str()))
	{
		if (ImGui::Button("Create child"))
		{
			go->CreateChild();
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("Delete"))
		{
			go->DeleteGameobject();
			//App->editor->DeleteSelected();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}*/

	if (go->showHierarchy && go->HasChilds())
	{		
		std::vector<GameObject*> goChilds = go->GetAllChilds();
		for (std::vector<GameObject*>::iterator it = goChilds.begin(); it != goChilds.end(); ++it)
		{
			DrawGameObject((*it));
		}
		ImGui::TreePop();
	}
}

void HierarchyWindow::NodeInput(GameObject* go)
{
	if (leftMouse == KEY_DOWN)
	{
		if (ImGui::IsItemHovered())
		{
			if (ctrl)
			{
				if (go->selected) App->editor->RemoveSelection(go);//go->selected = false;
				else App->editor->AddSelection(go);//go->selected = true;
			}
			else App->editor->SelectOne(go);
		}
		/*else
		{
			if (!ctrl) App->editor->RemoveSelection(go);
		}*/		
	}

	/*if (ImGui::IsItemHovered() && rightMouse == KEY_DOWN)
	{
		ImGui::OpenPopup(go->name.c_str());
	}*/
}