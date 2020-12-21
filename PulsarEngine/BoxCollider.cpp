#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "BoxCollider.h"
#include "PhysBody3D.h"
#include "Transform.h"
#include "Primitive.h"
#include "MathGeoLib/include/MathGeoLib.h"

BoxCollider::BoxCollider(GameObject* parent) : Component(parent, BOX_COLLIDER_COMP)
{
	component->boxCollider = this;
	App->physics->AddBody(this,float3::one);
	isTrigger = false;
	friction = 0.0f;
	mass = 10.0f;
	draw = true;
	body->UpdateTransform(gameobject->GetGlobalTransform());
}

BoxCollider::BoxCollider(GameObject* parent, float3 s) : Component(parent, BOX_COLLIDER_COMP)
{
	component->boxCollider = this;
	App->physics->AddBody(this,s);
	isTrigger = false;
	friction = 0.0f;
	mass = 10.0f;
	if(body != nullptr) body->scaleOffset = s;
	draw = true;
	body->UpdateTransform(gameobject->GetGlobalTransform());
}

BoxCollider::~BoxCollider()
{
	if (body != nullptr)
	{
		App->physics->RemoveCollider(UUID);
		body = nullptr;
	}
}

void BoxCollider::UpdateTransform()
{
	if(body != nullptr && gameobject->transformUpdate) body->UpdateTransform(gameobject->GetGlobalTransform());
}

void BoxCollider::UpdateComponent()
{
	if (draw && gameobject->selected)
	{
		if (body != nullptr) App->physics->DebugDrawBody(body->body);		
	}
}

void BoxCollider::PhysicsUpdate()
{
	if (body != nullptr && !body->isStatic)
	{
		gameobject->transform->SetPosition(body->GetPos());
		gameobject->transform->SetQuatRotation(body->GetRotation());
	}
}

void BoxCollider::SetStatic(bool val)
{
	if (body != nullptr)
	{
		body->SetStatic(val);
		if (!val) SetMass(mass);
	}
}

void BoxCollider::DeleteComponent()
{
	App->physics->RemoveCollider(UUID);	
	body = nullptr;
	delete this;
}

bool BoxCollider::IsStatic()
{
	if (body != nullptr) return body->isStatic;
	else return true;
}

float3 BoxCollider::GetSize()
{
	if (body != nullptr) return body->scaleOffset;
	else return float3::zero;
}

float3 BoxCollider::GetPosition()
{
	if (body != nullptr) return body->localOffset;
	else return float3::zero;
}

void BoxCollider::SetScale(float3 s)
{
	if (body != nullptr) body->SetScale(s.x,s.y,s.z);	
}

void BoxCollider::SetPos(float3 p)
{
	if (body != nullptr) body->SetPos(p.x,p.y,p.z);
}

void BoxCollider::SetMass(float val)
{
	if (val >= 0 && val < 100000000.0f && !body->isStatic)
	{
		if(body->SetMass(val)) mass = val;
	}
}

void BoxCollider::SetTrigger(bool val)
{
	isTrigger = val;
	body->SetTrigger(val);
}

void BoxCollider::SetFriction(float val)
{
	if (val >= 0 && val < 100.0f)
	{
		if (body->SetFriction(val)) friction = val;
	}
}

float* BoxCollider::GetTransform()
{
	if (body != nullptr) return body->GetTransform().ptr();
	else return gameobject->GetGlobalTransform().ptr();
}

float4x4 BoxCollider::GetTransformMat()
{
	if (body != nullptr) return body->GetTransform();
	else return gameobject->GetGlobalTransform();
}

void BoxCollider::SaveComponent(JSonHandler* file)
{
	JSonHandler node = file->InsertNodeArray("Components");
	node.SaveNum("CompType", (double)compType);
	node.SaveString("UUID", UUID.c_str());
	node.SaveBool("Active", active);
	node.SaveBool("Static", body->isStatic);
	node.SaveBool("Trigger", isTrigger);
	node.SaveBool("Draw",draw);
	node.SaveNum("Mass",mass);
	node.SaveNum("Friction", friction);
	//Position
	node.SaveNum("PosX", body->localOffset.x);
	node.SaveNum("PosY", body->localOffset.y);
	node.SaveNum("PosZ", body->localOffset.z);
	//Scale
	node.SaveNum("ScaleX", body->scaleOffset.x);
	node.SaveNum("ScaleY", body->scaleOffset.y);
	node.SaveNum("ScaleZ", body->scaleOffset.z);
}

void BoxCollider::LoadComponent(JSonHandler* file)
{
	App->physics->RemoveCollider(UUID);
	UUID = file->GetString("UUID");
	active = file->GetBool("Active");
	App->physics->AddBody(this, float3::one);
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
	body->SetPos(pos.x,pos.y,pos.z);
	//Scale
	float3 scale;
	scale.x = file->GetNum("ScaleX");
	scale.y = file->GetNum("ScaleY");
	scale.z = file->GetNum("ScaleZ");
	body->SetScale(scale.x,scale.y,scale.z);
}