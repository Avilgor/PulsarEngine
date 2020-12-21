#ifndef __CapsuleCollider_H__
#define __CapsuleCollider_H__

#include "Globals.h"
#include "Component.h"
#include "Primitive.h"
#include "MathGeoLib/include/MathGeoLib.h"

class CapsuleCollider : public Component
{
public:

	CapsuleCollider(GameObject* parent);
	CapsuleCollider(GameObject* parent, float rad,float height);
	~CapsuleCollider();

	void UpdateComponent();
	void PhysicsUpdate();
	void DeleteComponent();
	PhysBody3D* GetBody() { return body; }
	void SaveComponent(JSonHandler* file);
	void LoadComponent(JSonHandler* file);
	void UpdateTransform();
	float* GetTransform();
	float4x4 GetTransformMat();
	float3 GetSize();
	float3 GetPosition();
	bool IsStatic();
	void SetStatic(bool val);
	void SetScale(float r,float h);
	void SetPos(float3 pos);
	void SetMass(float val);
	void SetFriction(float val);
	void SetTrigger(bool val);

public:
	PhysBody3D* body = nullptr;
	bool draw;
	float mass;
	float friction;
	bool isTrigger;
};

#endif //__CapsuleCollider_H__