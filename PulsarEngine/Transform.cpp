#include "Globals.h"
#include "GameObject.h"
#include "Transform.h"
#include "MathGeoLib/include/MathGeoLib.h"


Transform::Transform(GameObject* parent) : Component(parent, TRANSFORM_COMP)
{
	transform = float4x4::FromTRS(float3::zero, Quat::identity, float3::one);
	transformT = transform.Transposed();
	UpdateEuler();
	updateTransform = true;
}

Transform::Transform(GameObject* parent, float3 pos, Quat rot, float3 scale) : Component(parent,TRANSFORM_COMP)
{
	transform = float4x4::FromTRS(pos, rot, scale);
	transformT = transform.Transposed();
	UpdateEuler();
	updateTransform = true;
}

Transform::Transform(GameObject* parent, float4x4 t) : Component(parent, TRANSFORM_COMP)
{
	transform = t;
	transform.Decompose(position, quaternionRotation, scale);
	transformT = transform.Transposed();
	UpdateEuler();
	updateTransform = true;
}

Transform::~Transform()
{

}

void Transform::UpdateComponent()
{
	if (updateTransform)
	{
		if (gameobject->GetParent() != nullptr) transformGlobal = gameobject->GetParent()->transform->GetGlobalTransform() * transform;
		transformTGlobal = transformGlobal.Transposed();
		UpdateTRS();
		updateTransform = false;
	}
}

void Transform::UpdateTRS()
{
	transform.Decompose(position, quaternionRotation, scale);
	UpdateEuler();
}


void Transform::ResetTransform()
{
	position = float3::zero;
	scale = float3::one;
	quaternionRotation = Quat::identity;
	
	UpdateEuler();
	updateTransform = true;
}

void Transform::UpdateEuler()
{
	eulerRotation = quaternionRotation.ToEulerXYZ();
	eulerRotation *= RADTODEG;
}

void Transform::UpdateLocal()
{
	transform = float4x4::FromTRS(position, quaternionRotation, scale);
	transformT = transform.Transposed();
}

void Transform::DeleteComponent()
{
	delete this;
}

float3 Transform::GetGlobalPosition()
{
	return float3(transformGlobal[0][3], transformGlobal[1][3], transformGlobal[2][3]);
}

void Transform::SetPosition(float3 pos)
{
	position = pos;
	UpdateLocal();
	updateTransform = true;
}

void Transform::SetScale(float3 s)
{
	scale = s;
	UpdateLocal();
	updateTransform = true;
}

void Transform::Rotate(float3 rot)
{
	eulerRotation += rot;
	float3 delta = eulerRotation * DEGTORAD;
	Quat quaternion_rotation = Quat::FromEulerXYZ(delta.x, delta.y, delta.z);
	quaternionRotation =  quaternion_rotation;
	UpdateLocal();
	updateTransform = true;
}

void Transform::SetQuatRotation(Quat rot)
{
	quaternionRotation = rot;
	UpdateEuler();
	UpdateLocal();
	updateTransform = true;
}

void Transform::SetEulerRotation(float3 degrees)
{
	eulerRotation = degrees;
	float3 delta = eulerRotation * DEGTORAD;
	Quat quaternion_rotation = Quat::FromEulerXYZ(delta.x, delta.y, delta.z);
	quaternionRotation = quaternion_rotation;
	UpdateLocal();
	updateTransform = true;
}

void Transform::SetGlobalTransform(float4x4 t)
{
	if (gameobject->GetParent() != nullptr) transform = gameobject->GetParent()->transform->GetGlobalTransform().Inverted() * t;
	else transform = t;

	transformGlobal = t;
	transformTGlobal = transformGlobal.Transposed();
	updateTransform = true;
}