#include "Globals.h"
#include "Application.h"
#include "HierarchyWindow.h"
#include "GameObject.h"
#include "Scene.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include <string>

HierarchyWindow::HierarchyWindow(std::string name) : EditorWindow(name)
{
}

HierarchyWindow::~HierarchyWindow()
{}


update_status HierarchyWindow::Draw()
{
	update_status ret = UPDATE_CONTINUE; 
	App->editor->hierarchyObjHover = nullptr;

	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::Begin(name.c_str(), &active);
	windowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
	/*ImGui::Text("Scene name:");
	ImGui::SameLine();
	char name[50];
	strcpy_s(name, 50, App->scene->GetActiveScene()->name.c_str());
	if (ImGui::InputText("###", name, 50, name_input_flags)) App->scene->GetActiveScene()->SetName(name);
	ImGui::Separator();*/
	DrawSceneGameObjects();
	ImGui::End();

	if (App->editor->leftMouse == KEY_UP && App->editor->dragObject != nullptr)
	{
		App->editor->dragObject = nullptr;
	}

	//if (App->editor->leftMouse == KEY_REPEAT && windowHovered) dragItem = true;
	//else dragItem = false;

	return ret;
}

void HierarchyWindow::DrawSceneGameObjects()
{
	std::vector<GameObject*> sceneObjects = App->scene->GetActiveScene()->GetRoot()->GetAllChilds();
	for (std::vector<GameObject*>::iterator it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
	{
		if(!(*it)->toDelete) DrawGameObject((*it));
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

	if (go->selected) nodeFlag |= ImGuiTreeNodeFlags_Selected;
	
	go->showHierarchy = ImGui::TreeNodeEx(go->name.c_str(), nodeFlag);

	if (go->active == false) ImGui::PopStyleColor();

	NodeInput(go);

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
	if (App->editor->leftMouse == KEY_DOWN)
	{
		if (ImGui::IsItemHovered()) App->editor->dragObject = go;		
	}

	if (App->editor->leftMouse == KEY_UP)
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
		{
			if (App->editor->dragObject != nullptr)
			{
				App->editor->SetParent(go,App->editor->dragObject);
				App->editor->dragObject = nullptr;
			}

			if (App->editor->ctrl == KEY_REPEAT)
			{
				if (go->selected) App->editor->RemoveSelection(go);
				else App->editor->AddSelection(go);
			}
			else App->editor->SelectOne(go);
		}
	}
}