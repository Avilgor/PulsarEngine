#ifndef __Component_H__
#define __Component_H__

#include <vector>
#include <string> 

class GameObject;
class Mesh;
class Material;
class Camera;
class JSonHandler;

class Component
{
public:

	Component(GameObject* parent, ComponentTypes type);
	~Component();

	virtual void UpdateComponent() {}
	virtual void DeleteComponent() { delete this; }
	void SetActive(bool val);
	Component* GetComponent() { return component; }
	Mesh* AsMesh() { return mesh; }
	Material* AsMaterial() { return material; }
	Camera* AsCamera() { return camera; }
	virtual void SaveComponent(JSonHandler* file) {}
	virtual void LoadComponent(JSonHandler* file) {}


public:
	GameObject* gameobject;
	ComponentTypes compType;	
	bool active;
	std::string UUID;
	Component* component = nullptr;
	Mesh* mesh = nullptr;
	Material* material = nullptr;
	Camera* camera = nullptr;
};

#endif //__Component_H__