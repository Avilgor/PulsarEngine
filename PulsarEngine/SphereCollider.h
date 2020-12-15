#ifndef __SphereCollider_H__
#define __SphereCollider_H__

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class SphereCollider : public Component
{
public:

	SphereCollider(GameObject* parent);
	SphereCollider(GameObject* parent, float rad);
	~SphereCollider();

	void UpdateComponent();
	void DeleteComponent();
	PhysBody3D* GetBody() { return body; }
	void SaveComponent(JSonHandler* file);
	void LoadComponent(JSonHandler* file);
	void UpdateTransform();
	float* GetTransform();

	void SetScale(float3 scale);
	void SetPos(float3 pos);

public:
	//btSphereShape* shape = nullptr;
	float rad;
	float3 scale;
	float3 pos;
	//float4x4 transform;
	PhysBody3D* body = nullptr;
	bool draw;
};

#endif //__SphereCollider_H__