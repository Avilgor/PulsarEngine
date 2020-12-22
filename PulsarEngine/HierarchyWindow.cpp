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
	DrawSceneGameObjects();
	ImGui::End();

	if (windowHovered && App->editor->leftMouse == KEY_UP && dragItem)
	{
		if (App->editor->hierarchyObjHover != nullptr && !App->editor->hierarchyObjHover->selected) App->editor->SetSelectionParent(App->editor->hierarchyObjHover);
	}

	if (App->editor->leftMouse == KEY_REPEAT && windowHovered) dragItem = true;
	else dragItem = false;

	return ret;
}

void HierarchyWindow::DrawSceneGameObjects()
{
	std::vector<GameObject*> sceneObjects = App->scene->GetActiveScene()->GetRoot()->GetAllChilds();
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
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
	{
		App->editor->hierarchyObjHover = go;
	}

	if (App->editor->leftMouse == KEY_DOWN)
	{
		if (ImGui::IsItemHovered())
		{
			if (App->editor->ctrl == KEY_REPEAT)
			{
				if (go->selected) App->editor->RemoveSelection(go);//go->selected = false;
				else App->editor->AddSelection(go);//go->selected = true;
			}
			else App->editor->SelectOne(go);
		}
	}
}