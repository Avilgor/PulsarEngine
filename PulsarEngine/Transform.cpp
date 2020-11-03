#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Transform.h"
#include "JSonHandler.h"
#include "MathGeoLib/include/MathGeoLib.h"


Transform::Transform(GameObject* parent) : Component(parent, TRANSFORM_COMP)
{
	transform = float4x4::FromTRS(float3::zero, Quat::identity, float3::one);
	transformT = transform.Transposed();
	UpdateTRS();
	UpdateEuler();
	UpdateComponent();
}

Transform::Transform(GameObject* parent, float3 pos, Quat rot, float3 scale) : Component(parent,TRANSFORM_COMP)
{
	transform = float4x4::FromTRS(pos, rot, scale);
	transformT = transform.Transposed();
	UpdateTRS();
	UpdateEuler();
	UpdateComponent();
}

Transform::Transform(GameObject* parent, float3 position, float3 rotation, float3 scale) : Component(parent, TRANSFORM_COMP)
{
	eulerRotation = rotation;
	UpdateQuaternion();
	transform = float4x4::FromTRS(position, quaternionRotation, scale);
	transformT = transform.Transposed();
	UpdateTRS();
	UpdateComponent();
}

Transform::Transform(GameObject* parent, float4x4 t) : Component(parent, TRANSFORM_COMP)
{
	transform = t;
	transform.Decompose(position, quaternionRotation, scale);
	transformT = transform.Transposed();
	UpdateTRS();
	UpdateEuler();
	UpdateComponent();
}

Transform::~Transform()
{

}

void Transform::UpdateComponent()
{
	if (updateTransform)
	{
		//Not working global, messing up all transform
		/*if (gameobject->GetParent() != nullptr) transformGlobal = gameobject->GetParent()->transform->GetGlobalTransform() * transform;
		transformTGlobal = transformGlobal.Transposed();*/

		/*if (gameobject->GetParent() != nullptr)
		{
			GameObject* parent = gameobject->GetParent();
			float3 pos = parent->transform->GetPosition();
			float3 euler = parent->transform->GetEulerRotation();
			float3 scal = parent->transform->GetScale();

			position += pos;
			eulerRotation += euler;
			scale += scal;
			UpdateQuaternion();
		}*/
		UpdateLocal();
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
	UpdateQuaternion();
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

void Transform::UpdateQuaternion()
{
	float3 delta = eulerRotation * DEGTORAD;
	Quat quaternion_rotation = Quat::FromEulerXYZ(delta.x, delta.y, delta.z);
	quaternionRotation = quaternion_rotation;
}

void Transform::SetEulerRotation(float3 degrees)
{
	eulerRotation = degrees;
	UpdateQuaternion();
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

void Transform::SaveComponent(JSonHandler* file)
{
	JSonHandler node = file->CreateNode("Transform");
	//Position
	node.CreateArray("Position");
	node.InsertNumArray("Position",position.x);
	node.InsertNumArray("Position", position.y);
	node.InsertNumArray("Position", position.z);
	//Rotation
	node.CreateArray("Rotation");
	node.InsertNumArray("Rotation", eulerRotation.x);
	node.InsertNumArray("Rotation", eulerRotation.y);
	node.InsertNumArray("Rotation", eulerRotation.z);
	//Size
	node.CreateArray("Scale");
	node.InsertNumArray("Scale", scale.x);
	node.InsertNumArray("Scale", scale.y);
	node.InsertNumArray("Scale", scale.z);
}

void Transform::LoadComponent(JSonHandler* file, const char* label)
{

}