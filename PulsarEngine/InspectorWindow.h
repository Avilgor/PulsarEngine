#ifndef __InspectorWindow_H__
#define __InspectorWindow_H__

#include "EditorWindow.h"
#include "Globals.h"
#include <string>

class BoxCollider;
class SphereCollider;
class CapsuleCollider;
class ConstraintPoint;

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
	void CameraSection(GameObject* go);
	void BoxColliderSection(BoxCollider* coll/*,int index*/);
	void SphereColliderSection(SphereCollider* coll/*,int index*/);
	void CapsuleColliderSection(CapsuleCollider* coll/*, int index*/);
	void PointConstraintSection(ConstraintPoint* point);
	/*void HingeConstraintSection();
	void SliderConstraintSection();
	void ConeConstraintSection();*/

private:
	ImGuiInputTextFlags name_input_flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
	GameObject* currentGo = nullptr;
};

#endif //__InspectorWindow_H__