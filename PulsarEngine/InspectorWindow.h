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
	void GameObjectSection();
	void TransformSection();
	void MeshSection();
	void MaterialSection();
};

#endif //__InspectorWindow_H__