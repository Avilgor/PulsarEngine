#include "Globals.h"
#include "Application.h"
#include "ProjectWindow.h"
#include "PathNode.h"
#include "Timer.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include <string>
#include <vector>

ProjectWindow::ProjectWindow(std::string name) : EditorWindow(name)
{
	GetTree();
	scanTimer.Start();
}

ProjectWindow::~ProjectWindow()
{}


update_status ProjectWindow::Draw()
{
	update_status ret = UPDATE_CONTINUE;
	
	if (App->editor->leftMouse == KEY_DOWN)
	{
		if (selectedItem != nullptr)
		{
			selectedItem->selectedNode = false;
			selectedItem = nullptr;
		}
	}

	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::Begin(name.c_str(), &active);
	mouseOnWindow = ImGui::IsWindowHovered();
	DrawItem(&assetsFolder);
	ImGui::End();

	if (selectedItem != nullptr)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && App->editor->mouse_in_scene)
		{
			App->fileSystem->GetDroppedFile(selectedItem->path.c_str());
		}
	}
	else if (scanTimer.ReadSec() > 3.0f)
	{
		PathNode temp = assetsFolder;
		GetTree();
		CheckFileMoved(temp);
		scanTimer.Start();
	}

	return ret;
}

void ProjectWindow::GetTree()
{
	std::vector<std::string> ignore_ext;
	ignore_ext.push_back("meta");
	assetsFolder = App->fileSystem->GetAllFiles("Assets", nullptr, &ignore_ext);
}

void ProjectWindow::DrawItem(PathNode* node)
{
	ImGuiTreeNodeFlags nodeFlag;
	if (node->isFile)
	{
		nodeFlag = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}
	else
	{
		if(node->children.empty()) nodeFlag = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		else nodeFlag = ImGuiTreeNodeFlags_OpenOnArrow;
	}
	

	//if (node->selectedNode) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 0.4));

	if (node->selectedNode)
	{
		nodeFlag |= ImGuiTreeNodeFlags_Selected;
	}

	node->treeOpen = ImGui::TreeNodeEx(node->localPath.c_str(), nodeFlag);

	//if (node.selectedNode == false) ImGui::PopStyleColor();

	NodeInput(node);


	if (node->treeOpen && !node->children.empty())
	{
		for (int i = 0; i < node->children.size(); i++)
		{
			DrawItem(&node->children[i]);
		}
		ImGui::TreePop();
	}
}

void ProjectWindow::NodeInput(PathNode* node)
{
	if (node->isFile)
	{
		if (App->editor->leftMouse == KEY_DOWN)
		{
			if (ImGui::IsItemHovered())
			{
				node->selectedNode = true;
				if (selectedItem != nullptr) selectedItem->selectedNode = false;
				selectedItem = node;
			}
		}
	}
}

void ProjectWindow::CheckFileMoved(PathNode node)
{
	if (node.isFile)
	{
		if (App->resourceManager->CheckMetaFile(node.localPath))
		{				
			App->resourceManager->CheckMetaPath(node.path, node.localPath);
		}
		else
		{
			//File does not have meta
			//New or renamed
			std::string extension = App->fileSystem->GetFileExtension(node.localPath.c_str());
			if (extension.compare("psmesh") == 0 || extension.compare("psmaterial") == 0 || extension.compare("psscene") == 0)
			{
				if (App->resourceManager->CreateResourceMetaByName(node.localPath))
				{
					LOG("Meta file %s created.",node.localPath.c_str());
				}
				else LOG("Meta file %s creation error.", node.localPath.c_str());
			}
		}
	}
	else if (!node.children.empty())
	{
		std::vector<PathNode> temp = node.children;
		for (int a = 0; a < temp.size(); a++)
		{			
			CheckFileMoved(temp[a]);
		}
	}
}

void ProjectWindow::PrintAllNodes(PathNode node)
{
	if (!node.children.empty())
	{
		std::vector<PathNode> temp = node.children;
		LOG("Files in %s: %d",node.path.c_str(), node.children.size());
		for (int a=0;a < temp.size();a++)
		{
			LOG("Name: %s", temp[a].localPath.c_str());
			PrintAllNodes(temp[a]);
		}
	}
}