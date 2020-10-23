#ifndef __GameObject_H__
#define __GameObject_H__

#include "Component.h"

#include <vector>
#include <string>

class Transform;

class GameObject
{
public:
	GameObject(const char* name = "Unknown", GameObject* parent = nullptr);
	~GameObject();

	void UpdateTransform();
	void UpdateGameObject();
	void SetActive(bool val);
	void AddComponent(ComponentTypes type);
	void DeleteGOComponent(ComponentTypes type);
	void AddChild(GameObject* child);
	void CreateChild();
	void DeleteChild(int id = 0);
	void RemoveChild(int id);
	void DeleteGameobject();
	std::vector<Component*> GetAllComponents() { return Components; }
	std::vector<GameObject*> GetAllChilds() { return Childs; }
	Component* GetFirstComponentType(ComponentTypes type);
	GameObject* GetFirstChilds() { return Childs[0]; }
	GameObject* GetParent() { return parent; }

public:
	std::string name;
	bool active;
	int ID;
	Transform* transform;

private:
	GameObject* parent = nullptr;
	std::vector<Component*> Components;
	std::vector<GameObject*> Childs;
};

#endif //__GameObject_H__