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
	rad = 1;
	draw = true;
	shape = new SpherePrimitive();
}

SphereCollider::SphereCollider(GameObject* parent, float r) : Component(parent, SPHERE_COLLIDER_COMP)
{
	component->sphereCollider = this;
	rad = r;
	App->physics->AddBody(this);
	draw = true;
	shape = new SpherePrimitive(r);
}

SphereCollider::~SphereCollider()
{
	App->physics->RemoveCollider(UUID);
	body = nullptr;
}

void SphereCollider::UpdateTransform()
{
	if (body != nullptr) body->UpdateTransform(gameobject->GetGlobalTransform());
}

void SphereCollider::UpdateComponent()
{
	UpdateTransform();
	if (draw && gameobject->selected)
	{
		if (shape != nullptr) shape->Render(body->GetTransform().Transposed());
	}
}

void SphereCollider::PhysicsUpdate()
{
	if (body != nullptr &&  !body->isStatic)
	{
		gameobject->transform->SetPosition(body->GetPos());
		//gameobject->transform->SetEulerRotation(body->GetRotation());
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
	if (body != nullptr)
	{
		float3 temp;
		temp.x = s;
		temp.y = s;
		temp.z = s;
		body->scaleOffset = temp;
		shape->radius = s;
	}
}

void SphereCollider::SetPos(float3 p)
{
	if (body != nullptr)
	{
		body->localOffset = p;
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
	//node.SaveBool("Active", active);
	//if (resMaterial != nullptr) node.SaveString("ResUUID", resMaterial->UUID.c_str());
	//else node.SaveString("ResUUID", "-1");
}

void SphereCollider::LoadComponent(JSonHandler* file)
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