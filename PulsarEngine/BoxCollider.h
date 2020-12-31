#ifndef __BoxCollider_H__
#define __BoxCollider_H__

#include "Globals.h"
#include "Component.h"
#include "Primitive.h"
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
	float4x4 GetTransformMat();
	void SetStatic(bool val);
	void SetScale(float3 scale);
	void SetPos(float3 pos);
	void SetMass(float val);
	void SetFriction(float val);
	void SetTrigger(bool val);
	float3 GetSize();
	float3 GetPosition();
	bool IsStatic();

	void ApplyForce(float3 dir,float force);
	
private:
	std::vector<float3> toapplyForcesDir;
	std::vector<float> toapplyForcesImpulse;

public:
	PhysBody3D* body = nullptr;
	bool draw;
	float mass;
	float friction;
	bool isTrigger;
};

#endif //__BoxCollider_H__