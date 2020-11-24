#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "JSonHandler.h"
#include "MathGeoLib/include/MathGeoLib.h"


Transform::Transform(GameObject* parent) : Component(parent, TRANSFORM_COMP)
{
	position = float3::zero;
	eulerRotation = float3::zero;;
	scale = float3::one;
	transform = float4x4::FromTRS(float3::zero, Quat::identity, float3::one);
	transformGlobal = float4x4::identity;
	loadedTransform = false;
	//updateTransform = true;
}

Transform::Transform(GameObject* parent, float3 pos, Quat rot, float3 scale) : Component(parent,TRANSFORM_COMP)
{
	position = float3::zero;
	eulerRotation = float3::zero;;
	scale = float3::one;
	transform = float4x4::FromTRS(pos, rot, scale);
	transformGlobal = float4x4::identity;
	loadedTransform = false;
	//updateTransform = true;
}

Transform::Transform(GameObject* parent, float3 position, float3 rotation, float3 scale) : Component(parent, TRANSFORM_COMP)
{
	position = float3::zero;
	eulerRotation = float3::zero;;
	scale = float3::one;
	eulerRotation = rotation;
	UpdateQuaternion();
	transform = float4x4::FromTRS(position, quaternionRotation, scale);
	transformGlobal = float4x4::identity;
	loadedTransform = false;
	//updateTransform = true;
}

Transform::Transform(GameObject* parent, float4x4 t) : Component(parent, TRANSFORM_COMP)
{
	position = float3::zero;
	eulerRotation = float3::zero;;
	scale = float3::one;
	transform = t;
	transform.Decompose(position, quaternionRotation, scale);
	transformGlobal = float4x4::identity;
	loadedTransform = false;
	//updateTransform = true;
}

Transform::~Transform()
{

}

float4x4 Transform::GetTransformTransposed()
{
	return transform.Transposed();
}

float4x4 Transform::GetGlobalTransformTransposed()
{
	return transformGlobal.Transposed();
}

void Transform::UpdateComponent()
{
	if (updateTransform)
	{
		if (gameobject->GetParent() != nullptr) transformGlobal = transform * gameobject->GetParent()->transform->GetGlobalTransform();
		
		UpdateTRS();
		gameobject->ToggleUpdateTransform();
		gameobject->UpdateAABB();
		if (gameobject->camera != nullptr) gameobject->camera->UpdateCamera(transformGlobal);
		updateTransform = false;
	}

	if (loadedTransform)
	{
		SetPosition(loadPos);
		SetEulerRotation(loadRot);
		SetScale(loadScale);
		loadedTransform = false;
		updateTransform = true;
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
	updateTransform = true;
}

void Transform::DeleteComponent()
{
	delete this;
}

float3 Transform::GetGlobalPosition()
{
	return float3(transformGlobal[0][3], transformGlobal[1][3], transformGlobal[2][3]);
}

float3 Transform::GetGlobalRotation()
{
	float3 position;
	float3 scale;
	Quat quat;
	transformGlobal.Decompose(position, quat, scale);
	float3 euler = quat.ToEulerXYZ();
	euler *= RADTODEG;
	return euler;
}

float3 Transform::GetGlobalScale()
{
	float3 position;
	float3 scale;
	Quat quat;
	transformGlobal.Decompose(position, quat, scale);
	return scale;
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
}

void Transform::Rotate(float3 rot)
{
	eulerRotation += rot;
	UpdateQuaternion();
	UpdateLocal();
}

void Transform::SetQuatRotation(Quat rot)
{
	quaternionRotation = rot;
	UpdateEuler();
	UpdateLocal();
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
}

void Transform::SetGlobalTransform()
{

	if (gameobject->GetParent() != nullptr)transform = gameobject->GetParent()->transform->GetGlobalTransform().Inverted() * transform;

	transformGlobal = transform;
	updateTransform = true;
}

void Transform::SetGlobalTransform(float4x4 t)
{	
	if (gameobject->GetParent() != nullptr)transform = t * gameobject->GetParent()->transform->GetGlobalTransform().Inverted();
	transformGlobal = t;
	updateTransform = true;
}

void Transform::SetLocalTransform(float4x4 t)
{
	transform = t;
	updateTransform = true;
}

void Transform::SaveComponent(JSonHandler* file)
{
	JSonHandler node = file->InsertNodeArray("Components");
	node.SaveNum("CompType", (double)compType);
	node.SaveString("UUID",UUID.c_str());
	//Position
	node.CreateArray("Position");
	node.InsertNumArray("Position", position.x);
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

void Transform::LoadComponent(JSonHandler* file)
{
	UUID = file->GetString("UUID");
	file->LoadArray("Position");
	file->LoadArray("Rotation");
	file->LoadArray("Scale");
	float3 pos((float)file->GetNumArray("Position", 0), (float)file->GetNumArray("Position", 1), (float)file->GetNumArray("Position", 2));
	loadPos = pos;

	float3 rot((float)file->GetNumArray("Rotation", 0), (float)file->GetNumArray("Rotation", 1),(float)file->GetNumArray("Rotation", 2));
	loadRot = rot;

	float3 s((float)file->GetNumArray("Scale", 0),(float)file->GetNumArray("Scale", 1),(float)file->GetNumArray("Scale", 2));
	loadScale = s;
	loadedTransform = true;
	updateTransform = true;
}