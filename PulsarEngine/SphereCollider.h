#ifndef __SphereCollider_H__
#define __SphereCollider_H__

#include "Globals.h"
#include "Component.h"
#include "Primitive.h"
#include "MathGeoLib/include/MathGeoLib.h"

class SphereCollider : public Component
{
public:

	SphereCollider(GameObject* parent);
	SphereCollider(GameObject* parent, float rad);
	~SphereCollider();

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
	void SetScale(float scale);
	void SetPos(float3 pos);
	void SetMass(float val);
	void SetFriction(float val);
	void SetTrigger(bool val);


	void ApplyForce(float3 dir, float force);

public:

	PhysBody3D* body = nullptr;
	bool draw;
	float mass;
	float friction;
	bool isTrigger;
};

#endif //__SphereCollider_H__