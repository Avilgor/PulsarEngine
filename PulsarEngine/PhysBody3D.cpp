#include "PhysBody3D.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include "MathGeoLib/include/MathGeoLib.h"

PhysBody3D::PhysBody3D(btRigidBody* body) : body(body)
{
	localOffset = float3::zero;
	scaleOffset = float3::zero;
}

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
	if(body != nullptr)
	{
		float* matrix;
		body->getWorldTransform().getOpenGLMatrix(matrix);
		return matrix;
	}

	return nullptr;
}

void PhysBody3D::SetTransform(float* matrix)
{
	if(body != nullptr && matrix != nullptr)
	{
		btTransform t;
		t.setFromOpenGLMatrix(matrix);
		body->setWorldTransform(t);
	}
}

void PhysBody3D::UpdateTransform(float4x4 globalMat)
{
	float3 position;
	float3 scale;
	Quat quat;
	globalMat.Decompose(position, quat, scale);
	position += localOffset;
	scale += localOffset;
	SetPos(position.x, position.y, position.z);
	SetScale(scale.x,scale.y,scale.z);
	SetRotation(quat);
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

void PhysBody3D::SetRotation(Quat rot)
{
	btTransform t = body->getWorldTransform();
	float3 euler = rot.ToEulerZYX();
	t.setRotation(btQuaternion(euler.z, euler.y, euler.x));
	body->setWorldTransform(t);
}

float3 PhysBody3D::GetPos()
{
	btVector3 pos = body->getWorldTransform().getOrigin();
	float3 temp;
	temp.x = pos.x();
	temp.y = pos.y();
	temp.z = pos.z();
	return temp;
}

float3 PhysBody3D::GetRotation()
{
	btQuaternion rot = body->getWorldTransform().getRotation();
	float3 temp;
	temp.x = rot.x();
	temp.y = rot.y();
	temp.z = rot.z();
	return temp;
}

void PhysBody3D::SetStatic(bool val)
{
	isStatic = val;
	if (val)
	{
		body->setFlags(1);
	}
	else
	{
		body->setFlags(2);
	}
}