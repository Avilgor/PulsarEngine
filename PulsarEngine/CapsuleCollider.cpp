#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "CapsuleCollider.h"
#include "PhysBody3D.h"
#include "Transform.h"
#include "Primitive.h"
#include "MathGeoLib/include/MathGeoLib.h"

CapsuleCollider::CapsuleCollider(GameObject* parent) : Component(parent, CAPSULE_COLLIDER_COMP)
{
	component->capsuleCollider = this;
	App->physics->AddBody(this);
	isTrigger = false;
	friction = 1.0f;
	mass = 10.0f;
	draw = true;
	body->UpdateTransform(gameobject->GetGlobalTransform());
}

CapsuleCollider::CapsuleCollider(GameObject* parent, float r, float h) : Component(parent, CAPSULE_COLLIDER_COMP)
{
	component->capsuleCollider = this;
	App->physics->AddBody(this);
	isTrigger = false;
	friction = 0.0f;
	mass = 10.0f;
	draw = true;
	if (body != nullptr)
	{
		body->scaleOffset.x = r;
		body->scaleOffset.y = h;
		body->scaleOffset.z = r;
	}
	body->UpdateTransform(gameobject->GetGlobalTransform());
}

CapsuleCollider::~CapsuleCollider()
{
	if (body != nullptr)
	{
		App->physics->RemoveCollider(UUID);
		body = nullptr;
	}
}

void CapsuleCollider::UpdateTransform()
{
	if (body != nullptr && gameobject->transformUpdate) body->UpdateTransform(gameobject->GetGlobalTransform());
}

void CapsuleCollider::UpdateComponent()
{
	if (draw && gameobject->selected)
	{
		if (body != nullptr) App->physics->DebugDrawBody(body->body);
	}
}

void CapsuleCollider::PhysicsUpdate()
{
	if (body != nullptr && !body->isStatic)
	{
		gameobject->transform->SetPosition(body->GetPos());
		gameobject->transform->SetQuatRotation(body->GetRotation());
	}
}

void CapsuleCollider::SetStatic(bool val)
{
	if (body != nullptr)
	{
		body->SetStatic(val);
		if (!val) SetMass(mass);
	}
}

void CapsuleCollider::DeleteComponent()
{
	App->physics->RemoveCollider(UUID);
	body = nullptr;
}

bool CapsuleCollider::IsStatic()
{
	if (body != nullptr) return body->isStatic;
	else return true;
}

float3 CapsuleCollider::GetSize()
{
	if (body != nullptr) return body->scaleOffset;
	else return float3::zero;
}

float3 CapsuleCollider::GetPosition()
{
	if (body != nullptr) return body->localOffset;
	else return float3::zero;
}

void CapsuleCollider::SetScale(float s, float h)
{
	if (body != nullptr) body->SetScale(s, h, s);	
}

void CapsuleCollider::SetPos(float3 p)
{
	if (body != nullptr) body->SetPos(p.x, p.y, p.z);		
}

void CapsuleCollider::SetMass(float val)
{
	if (val > 0 && val < 100000000.0f && !body->isStatic)
	{
		if (body->SetMass(val)) mass = val;
	}
}

void CapsuleCollider::SetTrigger(bool val)
{
	isTrigger = val;
	body->SetTrigger(val);
}

void CapsuleCollider::SetFriction(float val)
{
	if (val >= 0 && val < 100.0f)
	{
		if (body->SetFriction(val)) friction = val;
	}
}

float* CapsuleCollider::GetTransform()
{
	if (body != nullptr) return body->GetTransform().ptr();
	else return gameobject->GetGlobalTransform().ptr();
}

float4x4 CapsuleCollider::GetTransformMat()
{
	if (body != nullptr) return body->GetTransform();
	else return gameobject->GetGlobalTransform();
}

void CapsuleCollider::SaveComponent(JSonHandler* file)
{
	JSonHandler node = file->InsertNodeArray("Components");
	node.SaveNum("CompType", (double)compType);
	node.SaveString("UUID", UUID.c_str());
	node.SaveBool("Active", active);
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
	node.SaveNum("Heigth", body->scaleOffset.y);
}

void CapsuleCollider::LoadComponent(JSonHandler* file)
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
	SetScale(file->GetNum("Rad"), file->GetNum("Heigth"));
}