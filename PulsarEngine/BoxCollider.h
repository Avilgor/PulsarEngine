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
	float3 GetSize();
	float3 GetPosition();
	bool IsStatic();
	
public:
	//CubePrimitive* shape = nullptr;
	PhysBody3D* body = nullptr;
	bool draw;
	float mass;
};

#endif //__BoxCollider_H__