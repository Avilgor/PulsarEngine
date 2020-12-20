#include "Globals.h"
#include "PhysBody3D.h"
#include "Application.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include "MathGeoLib/include/MathGeoLib.h"

PhysBody3D::PhysBody3D(btRigidBody* body) : body(body)
{
	localOffset = float3::zero;
	scaleOffset = float3::one;
	transform = float4x4::zero;
	isStatic = false;
}

PhysBody3D::~PhysBody3D()
{
	delete body;
}

void PhysBody3D::Push(float x, float y, float z)
{
	body->applyCentralImpulse(btVector3(x, y, z));
}

float4x4 PhysBody3D::GetTransform()
{
	if(body != nullptr) return transform;	
	return float4x4::zero;
}

void PhysBody3D::UpdateTransform(float4x4 globalMat)
{
	float3 position;
	float3 scale;
	Quat quat;
	globalMat.Decompose(position, quat, scale);
	position += localOffset;
	scale += scaleOffset;

	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(position.x, position.y, position.z));	
	body->getCollisionShape()->setLocalScaling(btVector3(scale.x, scale.y, scale.z));

	btQuaternion quatB;
	quatB.setX(quat.x);
	quatB.setY(quat.y);
	quatB.setZ(quat.z);
	quatB.setW(quat.w);
	t.setRotation(quatB);
	body->setWorldTransform(t);

	transform = float4x4::FromTRS(position, quat, scale);
}

void PhysBody3D::SetPos(float x, float y, float z)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(x, y, z));
	body->setWorldTransform(t);
	localOffset.x = x;
	localOffset.y = y;
	localOffset.z = z;
}

void PhysBody3D::SetScale(float x, float y, float z)
{
	body->getCollisionShape()->setLocalScaling(btVector3(x, y, z));
	scaleOffset.x = x;
	scaleOffset.y = y;
	scaleOffset.z = z;
}

void PhysBody3D::SetRotation(Quat rot)
{
	btTransform t = body->getWorldTransform();
	btQuaternion quatB;
	quatB.setX(rot.x);
	quatB.setY(rot.y);
	quatB.setZ(rot.z);
	quatB.setW(rot.w);
	t.setRotation(quatB);
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
	temp.x = rot.getX();
	//if (temp.x < 0.001) temp.x = 0;
	temp.y = rot.getY();	
	//if (temp.y < 0.001) temp.y = 0;
	temp.z = rot.getZ();
	//if (temp.z < 0.001) temp.z = 0;
	//LOG("Rotations X:%f/Y:%f/Z:%f",temp.x,temp.y,temp.z);
	return temp;
}

void PhysBody3D::SetStatic(bool val)
{
	isStatic = val;
	if (val)
	{
		body->setFlags(body->getFlags() | 1);
		//body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CO_COLLISION_OBJECT);
		btVector3 zeroVector(0, 0, 0);
		body->setLinearVelocity(zeroVector);
		body->setAngularVelocity(zeroVector);
	}
	else
	{
		body->setFlags(body->getFlags() | 2);
		//body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CO_RIGID_BODY);
		btVector3 zeroVector(0, 0, 0);
		body->setLinearVelocity(zeroVector);
		body->setAngularVelocity(zeroVector);
	}
}