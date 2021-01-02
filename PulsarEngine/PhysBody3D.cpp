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
	defaultCollisionFlags = body->getCollisionFlags();
	defaultFlags = body->getFlags();
}

PhysBody3D::~PhysBody3D()
{
	if(body != nullptr) delete body;
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

bool PhysBody3D::SetMass(float val)
{
	if (!body->isActive()) body->activate(true);
	if (App->physics->runningSimulation == false)
	{
		btVector3 inertia;
		body->getCollisionShape()->calculateLocalInertia(val, inertia);
		body->setMassProps(val, inertia);
		body->updateInertiaTensor();
		return true;
	}

	return false;
}

bool PhysBody3D::SetFriction(float val)
{
	if (!body->isActive()) body->activate(true);
	if (App->physics->runningSimulation == false)
	{
		body->setFriction(val);
		return true;
	}
	return false;
}

void PhysBody3D::UpdateTransform(float4x4 globalMat)
{
	if (!body->isActive()) body->activate(true);

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
	body->getMotionState()->setWorldTransform(t);

	transform = float4x4::FromTRS(position, quat, scale);
}

void PhysBody3D::SetPos(float x, float y, float z)
{
	if (App->physics->runningSimulation == false)
	{
		btTransform t = body->getWorldTransform();		
		t.setOrigin(btVector3(x, y, z));
		body->setWorldTransform(t);
		body->getMotionState()->setWorldTransform(t);
		localOffset.x = x;
		localOffset.y = y;
		localOffset.z = z;
	}
}

void PhysBody3D::SetScale(float x, float y, float z)
{
	if (App->physics->runningSimulation == false)
	{	
		if (x > 0) scaleOffset.x = x;
		if (y > 0) scaleOffset.y = y;
		if (z > 0) scaleOffset.z = z;
		body->getCollisionShape()->setLocalScaling(btVector3(scaleOffset.x, scaleOffset.y, scaleOffset.z));
	}
}

void PhysBody3D::SetRotation(Quat rot)
{
	if (App->physics->runningSimulation == false)
	{
		btTransform t = body->getWorldTransform();
		btQuaternion quatB;
		quatB.setX(rot.x);
		quatB.setY(rot.y);
		quatB.setZ(rot.z);
		quatB.setW(rot.w);
		t.setRotation(quatB);
		body->setWorldTransform(t);
		body->getMotionState()->setWorldTransform(t);
	}
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

Quat PhysBody3D::GetRotation()
{
	btQuaternion rot = body->getWorldTransform().getRotation();
	Quat temp;
	temp.x = rot.getX();
	temp.y = rot.getY();	
	temp.z = rot.getZ();
	temp.w = rot.getW();
	return temp;
}

void PhysBody3D::SetStatic(bool val)
{
	isStatic = val;
	if (!body->isActive()) body->activate(true);
	if (val)
	{
		body->setFlags(body->CF_STATIC_OBJECT);
		btVector3 zeroVector(0, 0, 0);
		body->setLinearVelocity(zeroVector);
		body->setAngularVelocity(zeroVector);
		btVector3 inertia;
		body->getCollisionShape()->calculateLocalInertia(0.0f, inertia);
		body->setMassProps(0.0f, inertia);
		body->updateInertiaTensor();
	}
	else
	{
		body->setFlags(defaultFlags);
		btVector3 zeroVector(0, 0, 0);
		body->setLinearVelocity(zeroVector);
		body->setAngularVelocity(zeroVector);
	}
}

void PhysBody3D::SetTrigger(bool val)
{
	if (!body->isActive()) body->activate(true);
	if (val) body->setCollisionFlags(body->CO_GHOST_OBJECT);
	else body->setCollisionFlags(defaultCollisionFlags);
}