#ifndef __ProjectWindow_H__
#define __ProjectWindow_H__

#include "EditorWindow.h"
#include "Globals.h"
#include "PathNode.h"
#include "Timer.h"

#include <vector>

class ProjectWindow : public EditorWindow
{
public:
	ProjectWindow(std::string name);
	virtual ~ProjectWindow();

	update_status Draw();

	void GetTree();
	void DrawItem(PathNode* node);
	void PrintAllNodes(PathNode node);
	void NodeInput(PathNode* node);

private:
	void CheckFileMoved(PathNode node);

private:
	PathNode assetsFolder;
	//std::vector<PathNode> selectedItems;
};

#endif //__ProjectWindow_H__