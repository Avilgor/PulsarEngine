#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

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
	float* GetTransformPtr();
	void SetTransform(float* matrix);
	void SetPos(float x, float y, float z);
	void SetScale(float x, float y, float z);

public:
	GameObject* listener;
	btRigidBody* body;
};

#endif // __PhysBody3D_H__