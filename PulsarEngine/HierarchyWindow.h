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

private:
	bool dragItem;
	bool windowHovered;
	//ImGuiInputTextFlags name_input_flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
};

#endif //__HierarchyWindow_H__