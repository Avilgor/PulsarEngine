#include "PhysBody3D.h"
#include "Bullet/src/btBulletDynamicsCommon.h"
//#include "Bullet/include/btBulletDynamicsCommon.h"

PhysBody3D::PhysBody3D(btRigidBody* body) : body(body)
{}

PhysBody3D::~PhysBody3D()
{
	delete body;
}

void PhysBody3D::Push(float x, float y, float z)
{
	body->applyCentralImpulse(btVector3(x, y, z));
}

float* PhysBody3D::GetTransformPtr()
{
	if(body != NULL)
	{
		float* matrix;
		body->getWorldTransform().getOpenGLMatrix(matrix);
		return matrix;
	}

	return nullptr;
}

void PhysBody3D::SetTransform(float* matrix)
{
	if(body != NULL && matrix != NULL)
	{
		btTransform t;
		t.setFromOpenGLMatrix(matrix);
		body->setWorldTransform(t);
	}
}

void PhysBody3D::SetPos(float x, float y, float z)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(x, y, z));
	body->setWorldTransform(t);
	
}

void PhysBody3D::SetScale(float x, float y, float z)
{
	body->getCollisionShape()->setLocalScaling(btVector3(x, y, z));
}