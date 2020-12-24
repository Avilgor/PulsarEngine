#ifndef __Component_H__
#define __Component_H__

#include <vector>
#include <string> 

class GameObject;
class Mesh;
class Material;
class Camera;
class BoxCollider;
class SphereCollider;
class CapsuleCollider;
class JSonHandler;
class ConstraintPoint;
class ConstraintCone;
class ConstraintSlider;
class ConstraintHinge;

class Component
{
public:

	Component(GameObject* parent, ComponentTypes type);
	~Component();

	virtual void UpdateComponent() {}
	virtual void DeleteComponent() {}
	void SetActive(bool val);
	Component* GetComponent() { return component; }
	Mesh* AsMesh() { return mesh; }
	Material* AsMaterial() { return material; }
	Camera* AsCamera() { return camera; }
	SphereCollider* AsSphereCollider() { return sphereCollider; }
	BoxCollider* AsBoxCollider() { return boxCollider; }
	CapsuleCollider* AsCapsuleCollider() { return capsuleCollider; }
	ConstraintPoint* AsPointConstraint() { return pointConstraint; }
	ConstraintSlider* AsSliderConstraint() { return sliderConstraint; }
	ConstraintHinge* AsHingeConstraint() { return hingeConstraint; }
	ConstraintCone* AsConeConstraint() { return coneConstraint; }

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
	BoxCollider* boxCollider = nullptr;
	SphereCollider* sphereCollider = nullptr; 
	CapsuleCollider* capsuleCollider = nullptr;
	ConstraintPoint* pointConstraint = nullptr;
	ConstraintCone* coneConstraint = nullptr;
	ConstraintSlider* sliderConstraint = nullptr;
	ConstraintHinge* hingeConstraint = nullptr;
};

#endif //__Component_H__