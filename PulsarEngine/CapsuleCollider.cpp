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
	rad = 0.5f;
	height = 2.0f;
	draw = true;
	body->UpdateTransform(gameobject->GetGlobalTransform());
}

CapsuleCollider::CapsuleCollider(GameObject* parent, float r, float h) : Component(parent, CAPSULE_COLLIDER_COMP)
{
	component->capsuleCollider = this;
	rad = r;
	height = h;
	App->physics->AddBody(this);
	draw = true;
	body->UpdateTransform(gameobject->GetGlobalTransform());
}

CapsuleCollider::~CapsuleCollider()
{
	App->physics->RemoveCollider(UUID);
	body = nullptr;
}

void CapsuleCollider::UpdateTransform()
{
	if (body != nullptr && gameobject->transformUpdate) body->UpdateTransform(gameobject->GetGlobalTransform());
}

void CapsuleCollider::UpdateComponent()
{
	//if (App->physics->runningSimulation == false) UpdateTransform();
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
	if (body != nullptr) body->SetStatic(val);
}

void CapsuleCollider::DeleteComponent()
{
	App->physics->RemoveCollider(UUID);
	body = nullptr;
	delete this;
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
	if (body != nullptr)
	{
		body->SetScale(s, h, s);
		/*float3 temp;
		temp.x = s;
		temp.y = s;
		temp.z = s;
		body->scaleOffset = temp;*/
		//shape->radius = s;
	}
}

void CapsuleCollider::SetPos(float3 p)
{
	if (body != nullptr)
	{
		body->SetPos(p.x, p.y, p.z);
		//body->localOffset = p;
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
	//node.SaveBool("Active", active);
	//if (resMaterial != nullptr) node.SaveString("ResUUID", resMaterial->UUID.c_str());
	//else node.SaveString("ResUUID", "-1");
}

void CapsuleCollider::LoadComponent(JSonHandler* file)
{
	/*Mesh* meshComp = nullptr;
	if (gameobject->GetFirstComponentType(MESH_COMP) != nullptr) meshComp = gameobject->GetFirstComponentType(MESH_COMP)->AsMesh();
	UUID = file->GetString("UUID");
	active = file->GetBool("Active");
	EngineResource* res = App->resourceManager->GetResource(file->GetString("ResUUID"));
	if (res != nullptr)
	{
		resMaterial = res->AsMaterial();
		resMaterial->references++;
	}
	if (resMaterial != nullptr) meshComp->SetMaterial(resMaterial);*/
}