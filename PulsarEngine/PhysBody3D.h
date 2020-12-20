#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "MathGeoLib/include/MathGeoLib.h"

#include <vector>

class btRigidBody;
class GameObject;

struct PhysBody3D
{
	friend class ModulePhysics;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	float4x4 GetTransform();
	//void SetTransform(float* matrix);
	void SetPos(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void UpdateTransform(float4x4 globalMat);
	void SetRotation(Quat rot);
	void SetStatic(bool val);
	float3 GetPos();
	Quat GetRotation();

private:
	void UpdateTransform();

public:
	GameObject* listener;
	btRigidBody* body;
	bool isStatic;
	float3 localOffset;
	float3 scaleOffset;

private:
	float4x4 transform;
};

#endif // __PhysBody3D_H__