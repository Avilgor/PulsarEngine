#ifndef __GameObject_H__
#define __GameObject_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include <vector>
#include <string>

class Transform;

class GameObject
{
public:
	GameObject(const char* name = "Unknown", GameObject* parent = nullptr);
	GameObject(const char* name, float3 pos, Quat rotation = Quat::identity, float3 scale = float3::one,GameObject* parent = nullptr);
	GameObject(const char* name, float3 pos, float3 rotation = float3::zero, float3 scale = float3::one, GameObject* parent = nullptr);
	~GameObject();

	void UpdateTransform();
	void UpdateGameObject();
	void DrawMesh();
	void SetActive(bool val);
	void AddComponent(ComponentTypes type);
	void Delete();
	void SaveToDelete(GameObject* trash);
	void DeleteGOComponent(ComponentTypes type);
	void AddChild(GameObject* child);
	void CreateChild();
	void DeleteChild(std::string id);
	void RemoveChild(std::string id);
	void DeleteAllChilds();
	void DeleteGameobject();
	bool HasChilds();
	void SetTransformUpdate(bool val) { transformUpdate = val; }
	void SetParent(GameObject* parent);
	void UpdateAABB();
	std::vector<Component*> GetAllComponents() { return Components; }
	std::vector<GameObject*> GetAllChilds() { return Childs; }
	Component* GetFirstComponentType(ComponentTypes type);
	GameObject* GetFirstChild() { return Childs[0]; }
	GameObject* GetParent() { return parent; }

private:
	void AddPendingChilds();
	void CreateTransform();
	void CreateTransform(float3 pos, Quat rotation = Quat::identity, float3 scale = float3::one);
	void CreateTransform(float3 pos, float3 rotation = float3::zero, float3 scale = float3::one);
public:
	std::string name;
	bool active;
	std::string UUID;
	bool selected;
	bool showHierarchy;
	bool toDelete;
	bool transformUpdate;
	Transform* transform;

private:
	GameObject* parent = nullptr;
	std::vector<Component*> Components;
	std::vector<GameObject*> Childs;
	std::vector<GameObject*> toDeleteChilds;
	std::vector<GameObject*> toAddChilds;

};

#endif //__GameObject_H__