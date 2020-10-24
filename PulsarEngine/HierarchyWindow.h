#ifndef __HierarchyWindow_H__
#define __HierarchyWindow_H__

#include "EditorWindow.h"
#include "Globals.h"
#include <string>

class HierarchyWindow : public EditorWindow
{
public:
	HierarchyWindow(std::string name);
	virtual ~HierarchyWindow();

	update_status Draw();
	void DrawSceneGameObjects();
	void DrawGameObject(GameObject* go);
	void NodeInput(GameObject* go);
	void SaveInputs();

private:
	bool ctrl;
	KEY_STATE rightMouse;
	KEY_STATE leftMouse;
};

#endif //__HierarchyWindow_H__