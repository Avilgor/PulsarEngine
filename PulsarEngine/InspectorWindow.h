#ifndef __InspectorWindow_H__
#define __InspectorWindow_H__

#include "EditorWindow.h"
#include "Globals.h"
#include <string>

class InspectorWindow : public EditorWindow
{
public:
	InspectorWindow(std::string name);
	virtual ~InspectorWindow();

	update_status Draw();

private:
	void GameObjectSection(GameObject* go);
	void TransformSection(GameObject* go);
	void MeshSection(GameObject* go);
	void MaterialSection(GameObject* go);

public:

private:
	ImGuiInputTextFlags name_input_flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
};

#endif //__InspectorWindow_H__