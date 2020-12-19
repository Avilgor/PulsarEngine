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
	float3 GetSize();
	float3 GetPosition();
	bool IsStatic();
	void SetStatic(bool val);
	void SetScale(float scale);
	void SetPos(float3 pos);

public:
	SpherePrimitive* shape = nullptr; 
	float rad;
	PhysBody3D* body = nullptr;
	bool draw;
};

#endif //__SphereCollider_H__