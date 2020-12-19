#ifndef __BoxCollider_H__
#define __BoxCollider_H__

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class BoxCollider : public Component
{
public:

	BoxCollider(GameObject* parent);
	BoxCollider(GameObject* parent,float3 scale);
	~BoxCollider();

	void UpdateComponent();
	void PhysicsUpdate();
	void DeleteComponent();
	PhysBody3D* GetBody() { return body; }
	void SaveComponent(JSonHandler* file);
	void LoadComponent(JSonHandler* file);
	void UpdateTransform();
	float* GetTransform();
	void SetStatic(bool val);
	void SetScale(float3 scale);
	void SetPos(float3 pos);
	float3 GetSize();
	float3 GetPosition();
	bool IsStatic();
	
public:
	//btBoxShape* shape = nullptr;
	PhysBody3D* body = nullptr;
	bool draw;
};

#endif //__BoxCollider_H__