#include "RigidBody.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

RigidBody::RigidBody(GameObject* parent,btRigidBody* b): Component(parent, BOX_COLLIDER_COMP)
{
	body = b;
	component->rigidbody = this;
}

RigidBody::~RigidBody()
{
	delete body;
}

void RigidBody::Push(float x, float y, float z)
{
	body->applyCentralImpulse(btVector3(x, y, z));
}

void RigidBody::GetTransform(float* matrix) const
{
	if (body != NULL && matrix != NULL)
	{
		body->getWorldTransform().getOpenGLMatrix(matrix);
	}
}

void RigidBody::SetTransform(float* matrix)
{
	if (body != NULL && matrix != NULL)
	{
		btTransform t;
		t.setFromOpenGLMatrix(matrix);
		body->setWorldTransform(t);
	}
}

void RigidBody::SetPos(float x, float y, float z)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(x, y, z));
	body->setWorldTransform(t);
}