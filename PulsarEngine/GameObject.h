#ifndef __GameObject_H__
#define __GameObject_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/Geometry/AABB.h"

#include <vector>
#include <string>

class Transform;
class Camera;

class GameObject
{
public:
	GameObject(const char* name = "Unknown", GameObject* parent = nullptr);
	GameObject(const char* name, float3 pos, Quat rotation = Quat::identity, float3 scale = float3::one,GameObject* parent = nullptr);
	GameObject(const char* name, float3 pos, float3 rotation = float3::zero, float3 scale = float3::one, GameObject* parent = nullptr);
	~GameObject();

	void UpdateTransform();
	void ToggleUpdateTransform();
	void UpdateGameObject();
	void SetUUID(const char* id);
	void DrawMesh();
	void SetActive(bool val);
	Component* AddComponent(ComponentTypes type);
	void Delete();
	void SaveToDelete(GameObject* trash);
	void DeleteGOComponent(ComponentTypes type);

	void AddChild(GameObject* child);
	GameObject* CreateChild();
	GameObject* CreateChild(const char* name);
	void DeleteChild(std::string id);
	void RemoveChild(std::string id);
	void DeleteAllChilds();
	void DeleteGameobject();
	bool HasChilds();

	void SetTransformUpdate(bool val) { transformUpdate = val; }
	void SetParent(GameObject* parent);
	std::vector<Component*> GetAllComponents() { return Components; }
	std::vector<GameObject*> GetAllChilds() { return Childs; }
	Component* GetFirstComponentType(ComponentTypes type);
	GameObject* GetFirstChild() { return Childs[0]; }
	GameObject* GetParent() { return parent; }

	void SaveGameobject(JSonHandler* file, const char* label);
	void LoadGameObject(JSonHandler* file);
	void LoadTransform(JSonHandler* file);

	void UpdateAABB();
	AABB GetGlobalAABB();
	void SetDrawAABB(bool val);
	void CheckRayIntersect(std::vector<GameObject*> *vec,LineSegment ray);
	GameObject* CheckRayIntersect(LineSegment ray);

private:
	void AddPendingChilds();
	void CreateTransform();
	void CreateTransform(float3 pos, Quat rotation = Quat::identity, float3 scale = float3::one);
	void CreateTransform(float3 pos, float3 rotation = float3::zero, float3 scale = float3::one);
public:
	std::string name = "";
	bool active = true;
	std::string UUID = "";
	std::string parentUUID = "";
	bool selected = false;
	bool showHierarchy = false;
	bool toDelete = false;
	bool transformUpdate = true;
	bool drawAABB = false;
	Transform* transform = nullptr;
	Camera* camera = nullptr;
	AABB Gaabb;
	OBB Gobb;

private:
	GameObject* parent = nullptr;
	std::vector<Component*> Components;
	std::vector<GameObject*> Childs;
	std::vector<GameObject*> toDeleteChilds;
	std::vector<GameObject*> toAddChilds;
	bool hasAABB;
};

#endif //__GameObject_H__