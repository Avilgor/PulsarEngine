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

public:
	bool mouseOnWindow = false;

private:
	PathNode assetsFolder;
	PathNode* selectedItem = nullptr;
	Timer scanTimer;
};

#endif //__ProjectWindow_H__