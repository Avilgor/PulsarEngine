#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "SphereCollider.h"
#include "PhysBody3D.h"
#include "Transform.h"
#include "Primitive.h"
#include "MathGeoLib/include/MathGeoLib.h"

SphereCollider::SphereCollider(GameObject* parent) : Component(parent, SPHERE_COLLIDER_COMP)
{
	component->sphereCollider = this;
	App->physics->AddBody(this);
	draw = true;
	isTrigger = false;
	friction = 0.0f;
	mass = 10.0f;
	body->UpdateTransform(gameobject->GetGlobalTransform());
}

SphereCollider::SphereCollider(GameObject* parent, float r) : Component(parent, SPHERE_COLLIDER_COMP)
{
	component->sphereCollider = this;
	App->physics->AddBody(this);
	draw = true;
	isTrigger = false;
	friction = 0.0f;
	mass = 10.0f;
	if (body != nullptr)
	{
		body->scaleOffset.x = r;
		body->scaleOffset.y = r;
		body->scaleOffset.z = r;
	}
	body->UpdateTransform(gameobject->GetGlobalTransform());
}

SphereCollider::~SphereCollider()
{
	if (body != nullptr)
	{
		App->physics->RemoveCollider(UUID);
		body = nullptr;
	}
}

void SphereCollider::UpdateTransform()
{
	if (body != nullptr && gameobject->transformUpdate) body->UpdateTransform(gameobject->GetGlobalTransform());
}

void SphereCollider::UpdateComponent()
{
	if (draw && gameobject->selected)
	{
		if (body != nullptr) App->physics->DebugDrawBody(body->body);
	}
}

void SphereCollider::PhysicsUpdate()
{
	if (body != nullptr &&  !body->isStatic)
	{
		gameobject->transform->SetPosition(body->GetPos());
		gameobject->transform->SetQuatRotation(body->GetRotation());
	}
}

void SphereCollider::SetStatic(bool val)
{
	if (body != nullptr) body->SetStatic(val);
}

void SphereCollider::DeleteComponent()
{
	App->physics->RemoveCollider(UUID);
	body = nullptr;
	delete this;
}

bool SphereCollider::IsStatic()
{
	if (body != nullptr) return body->isStatic;
	else return true;
}

float3 SphereCollider::GetSize()
{
	if (body != nullptr) return body->scaleOffset;
	else return float3::zero;
}

float3 SphereCollider::GetPosition()
{
	if (body != nullptr) return body->localOffset;
	else return float3::zero;
}

void SphereCollider::SetScale(float s)
{
	if (body != nullptr) body->SetScale(s, s, s);		
}

void SphereCollider::SetPos(float3 p)
{
	if (body != nullptr) body->SetPos(p.x, p.y, p.z);		
}

void SphereCollider::SetMass(float val)
{
	if (val > 0 && val < 100000000.0f)
	{
		if (body->SetMass(val)) mass = val;
	}
}

void SphereCollider::SetTrigger(bool val)
{
	isTrigger = val;
	body->SetTrigger(val);
}

void SphereCollider::SetFriction(float val)
{
	if (val >= 0 && val < 100.0f)
	{
		if (body->SetFriction(val)) friction = val;
	}
}

float* SphereCollider::GetTransform()
{
	if (body != nullptr) return body->GetTransform().ptr();
	else return gameobject->GetGlobalTransform().ptr();
}

float4x4 SphereCollider::GetTransformMat()
{
	if (body != nullptr) return body->GetTransform();
	else return gameobject->GetGlobalTransform();
}

void SphereCollider::SaveComponent(JSonHandler* file)
{
	JSonHandler node = file->InsertNodeArray("Components");
	node.SaveNum("CompType", (double)compType);
	node.SaveString("UUID", UUID.c_str());
	node.SaveBool("Static", body->isStatic);
	node.SaveBool("Trigger", isTrigger);
	node.SaveBool("Draw", draw);
	node.SaveNum("Mass", mass);
	node.SaveNum("Friction", friction);
	//Position
	node.SaveNum("PosX", body->localOffset.x);
	node.SaveNum("PosY", body->localOffset.y);
	node.SaveNum("PosZ", body->localOffset.z);
	//Scale
	node.SaveNum("Rad", body->scaleOffset.x);
	//Rotation
	/*Quat rot = body->GetRotation();
	node.SaveNum("QuatX", rot.x);
	node.SaveNum("QuatY", rot.y);
	node.SaveNum("QuatZ", rot.z);
	node.SaveNum("QuatW", rot.w);*/
}

void SphereCollider::LoadComponent(JSonHandler* file)
{
	App->physics->RemoveCollider(UUID);
	UUID = file->GetString("UUID");
	active = file->GetBool("Active");
	App->physics->AddBody(this);
	body->UpdateTransform(gameobject->GetGlobalTransform());
	draw = file->GetBool("Draw");
	body->SetStatic(file->GetBool("Static"));
	SetMass(file->GetNum("Mass"));
	SetFriction(file->GetNum("Friction"));
	SetTrigger(file->GetBool("Trigger"));
	//Position
	float3 pos;
	pos.x = file->GetNum("PosX");
	pos.y = file->GetNum("PosY");
	pos.z = file->GetNum("PosZ");
	body->SetPos(pos.x, pos.y, pos.z);
	//Scale
	SetScale(file->GetNum("Rad"));
	//Rotation
	/*Quat q;
	q.x = file->GetNum("QuatX");
	q.y = file->GetNum("QuatY");
	q.z = file->GetNum("QuatZ");
	q.w = file->GetNum("QuatW");
	body->SetRotation(q);*/
}