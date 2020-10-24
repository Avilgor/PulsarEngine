#include "Globals.h"
#include "GameObject.h"
#include "Transform.h"
#include "MathGeoLib/include/MathGeoLib.h"


Transform::Transform(GameObject* parent) : Component(parent, TRANSFORM_COMP)
{
	transform = float4x4::FromTRS(float3::zero, Quat::identity, float3::one);
	needUpdate = true;
}

Transform::Transform(GameObject* parent, float3 pos, Quat rot, float3 scale) : Component(parent,TRANSFORM_COMP)
{
	transform = float4x4::FromTRS(pos, rot, scale);
	needUpdate = true;
}

Transform::Transform(GameObject* parent, float4x4 t) : Component(parent, TRANSFORM_COMP)
{
	transform = t;
	transform.Decompose(position, quaternionRotation, scale);
	needUpdate = true;
}

Transform::~Transform()
{

}

void Transform::UpdateComponent()
{
	if (needUpdate)
	{
		if (gameobject->GetParent() != nullptr) transformGlobal = gameobject->GetParent()->transform->transformGlobal * transform;
		else transformGlobal = transform;
		transformTGlobal = transformGlobal.Transposed();
		UpdateTransform();
	}
}

void Transform::UpdateTransform()
{
	transform.Decompose(position, quaternionRotation, scale);
	eulerRotation = quaternionRotation.ToEulerXYZ();
	eulerRotation *= RADTODEG;
	needUpdate = false;
}

void Transform::ResetTransform()
{
	position = float3::zero;
	scale = float3::one;
	quaternionRotation = Quat::identity;

	eulerRotation = quaternionRotation.ToEulerXYZ();
	eulerRotation *= RADTODEG;
	UpdateLocal();

	if ((scale.x * scale.y * scale.z) >= 0) normalsFlipped = false;
	else normalsFlipped = true;
}

void Transform::UpdateLocal()
{
	transform = float4x4::FromTRS(position, quaternionRotation, scale);
	needUpdate = true;
}

void Transform::DeleteComponent()
{
	delete this;
}

float3 Transform::GetGlobalPosition()
{
	return float3(transformTGlobal[0][3], transformTGlobal[1][3], transformTGlobal[2][3]);
}

void Transform::SetPosition(float3 pos)
{
	position = pos;
	UpdateLocal();
}

void Transform::SetScale(float3 s)
{
	scale = s;
	UpdateLocal();

	if ((scale.x * scale.y * scale.z) >= 0) normalsFlipped = false;
	else normalsFlipped = true;
}

void Transform::Rotate(float3 rot)
{
	eulerRotation += rot;
	float3 delta = eulerRotation * DEGTORAD;
	Quat quaternion_rotation = Quat::FromEulerXYZ(delta.x, delta.y, delta.z);
	quaternionRotation =  quaternion_rotation;
	UpdateLocal();
}

void Transform::SetQuatRotation(Quat rot)
{
	quaternionRotation = rot;
	eulerRotation = quaternionRotation.ToEulerXYZ();
	eulerRotation *= RADTODEG;
	UpdateLocal();
}

void Transform::SetEulerRotation(float3 degrees)
{
	eulerRotation = degrees;
	float3 delta = eulerRotation * DEGTORAD;
	Quat quaternion_rotation = Quat::FromEulerXYZ(delta.x, delta.y, delta.z);
	quaternionRotation = quaternion_rotation;
	UpdateLocal();
}

void Transform::SetTransform(float4x4 t)
{
	float4x4 localTransform;
	if (gameobject->GetParent() != nullptr) localTransform = gameobject->GetParent()->transform->transformGlobal.Inverted() * t;
	else localTransform = t;

	transform = localTransform;
	transformGlobal = t;
	transformTGlobal = transformGlobal.Transposed();
}