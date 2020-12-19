#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "BoxCollider.h"
#include "PhysBody3D.h"
#include "Transform.h"
#include "MathGeoLib/include/MathGeoLib.h"

BoxCollider::BoxCollider(GameObject* parent) : Component(parent, BOX_COLLIDER_COMP)
{
	component->boxCollider = this;
	App->physics->AddBody(this);
	draw = true;
}

BoxCollider::BoxCollider(GameObject* parent, float3 s) : Component(parent, BOX_COLLIDER_COMP)
{
	component->boxCollider = this;
	App->physics->AddBody(this);
	if(body != nullptr) body->scaleOffset = s;
	draw = true;
}

BoxCollider::~BoxCollider()
{
	App->physics->RemoveCollider(UUID);
	body = nullptr;
}

void BoxCollider::UpdateTransform()
{
	body->UpdateTransform(gameobject->GetGlobalTransform());
}

void BoxCollider::UpdateComponent()
{
	UpdateTransform();
	/*if (draw && gameobject->selected)
	{
		float3 corners[8];
		corners[0] = shape->getVertex();
		corners[1] = frustum.CornerPoint(1);
		corners[2] = frustum.CornerPoint(2);
		corners[3] = frustum.CornerPoint(3);
		corners[4] = frustum.CornerPoint(4);
		corners[5] = frustum.CornerPoint(5);
		corners[6] = frustum.CornerPoint(6);
		corners[7] = frustum.CornerPoint(7);
		App->renderer3D->RenderBox(corners);
	}*/
}

void BoxCollider::PhysicsUpdate()
{
	if (!body->isStatic)
	{
		//LOG("Physics update Box collider");
		gameobject->transform->SetPosition(body->GetPos());
		gameobject->transform->SetEulerRotation(body->GetRotation());
	}
}

void BoxCollider::SetStatic(bool val)
{
	body->SetStatic(val);
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
	if (body != nullptr)
	{
		body->scaleOffset = s;
	}
}

void BoxCollider::SetPos(float3 p)
{
	if (body != nullptr)
	{
		body->localOffset = p;
	}
}

float* BoxCollider::GetTransform()
{
	if (body != nullptr) return body->GetTransformPtr();
	else return gameobject->GetGlobalTransform().ptr();
}

void BoxCollider::SaveComponent(JSonHandler* file)
{
	JSonHandler node = file->InsertNodeArray("Components");
	node.SaveNum("CompType", (double)compType);
	node.SaveString("UUID", UUID.c_str());
	//node.SaveBool("Active", active);
	//if (resMaterial != nullptr) node.SaveString("ResUUID", resMaterial->UUID.c_str());
	//else node.SaveString("ResUUID", "-1");
}

void BoxCollider::LoadComponent(JSonHandler* file)
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