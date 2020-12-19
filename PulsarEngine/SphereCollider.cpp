#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "SphereCollider.h"
#include "PhysBody3D.h"
#include "Transform.h"
#include "MathGeoLib/include/MathGeoLib.h"

SphereCollider::SphereCollider(GameObject* parent) : Component(parent, SPHERE_COLLIDER_COMP)
{
	component->sphereCollider = this;
	App->physics->AddBody(this);
	rad = 1;
	draw = true;
}

SphereCollider::SphereCollider(GameObject* parent, float r) : Component(parent, SPHERE_COLLIDER_COMP)
{
	component->sphereCollider = this;
	rad = r;
	App->physics->AddBody(this);
	draw = true;
}

SphereCollider::~SphereCollider()
{
	App->physics->RemoveCollider(UUID);
	body = nullptr;
}

void SphereCollider::UpdateTransform()
{
	body->UpdateTransform(gameobject->GetGlobalTransform());
}

void SphereCollider::UpdateComponent()
{
	UpdateTransform();
	/*if (draw && gameobject->selected)
	{
		shape->getNumVertices();
		float3 corners[8];
		corners[0] = shape->getVertex();
		corners[1] = frustum.CornerPoint(1);
		corners[2] = frustum.CornerPoint(2);
		corners[3] = frustum.CornerPoint(3);
		corners[4] = frustum.CornerPoint(4);
		corners[5] = frustum.CornerPoint(5);
		corners[6] = frustum.CornerPoint(6);
		corners[7] = frustum.CornerPoint(7);
		App->renderer3D->RenderBox
	}*/
}

void SphereCollider::PhysicsUpdate()
{
	if (!body->isStatic)
	{
		//LOG("Physics update Sphere collider");
		gameobject->transform->SetPosition(body->GetPos());
		gameobject->transform->SetEulerRotation(body->GetRotation());
	}
}

void SphereCollider::SetStatic(bool val)
{
	body->SetStatic(val);
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

void SphereCollider::SetScale(float3 s)
{
	if (body != nullptr)
	{
		body->scaleOffset = s;
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
	if (body != nullptr) return body->GetTransformPtr();
	else return gameobject->GetGlobalTransform().ptr();
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