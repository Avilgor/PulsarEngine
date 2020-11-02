#ifndef __Component_H__
#define __Component_H__

#include <vector>

class GameObject;
class Mesh;
class Material;

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
	virtual uint OnSave(char** buffer) { return 0; }
	virtual void OnLoad() {}


public:
	GameObject* gameobject;
	ComponentTypes compType;	
	bool active;

	Component* component = nullptr;
	Mesh* mesh = nullptr;
	Material* material = nullptr;
};

#endif //__Component_H__