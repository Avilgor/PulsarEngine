#ifndef __RigidBody_H__
#define __RigidBody_H__

#include "Globals.h"
#include "Color.h"
#include "Component.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

class btRigidBody;
class GameObject;

class RigidBody : public Component
{
public:
	RigidBody(GameObject* parent, btRigidBody* body);
	~RigidBody();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(float* matrix);
	void SetPos(float x, float y, float z);

public:
	GameObject* listener;
	btRigidBody* body;
};

#endif // __RigidBody_H__