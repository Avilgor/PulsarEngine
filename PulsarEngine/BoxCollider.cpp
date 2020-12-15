#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "BoxCollider.h"
#include "PhysBody3D.h"
#include "MathGeoLib/include/MathGeoLib.h"

BoxCollider::BoxCollider(GameObject* parent) : Component(parent, BOX_COLLIDER_COMP)
{
	component->boxCollider = this;
	App->physics->AddBody(this);
	scale = float3::one;
	draw = true;
}

BoxCollider::BoxCollider(GameObject* parent, float3 s) : Component(parent, BOX_COLLIDER_COMP)
{
	component->boxCollider = this;
	scale = s;
	App->physics->AddBody(this);
	draw = true;
}

BoxCollider::~BoxCollider()
{
	App->physics->RemoveCollider(UUID);
	body = nullptr;
}

void BoxCollider::UpdateTransform()
{
	//float3 tempPos;
	//float3 tempScale;
	//Quat tempRot;
	//float4x4 tempT = gameobject->transform->GetGlobalTransform();
	//tempT.Decompose(tempPos, tempRot, tempScale);
	//pos += tempPos;
	//scale += tempScale;
	//transform = float4x4::FromTRS(pos, tempRot, scale);
}

void BoxCollider::UpdateComponent()
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

void BoxCollider::DeleteComponent()
{
	App->physics->RemoveCollider(UUID);	
	body = nullptr;
	delete this;
}

void BoxCollider::SetScale(float3 s)
{
	if (body != nullptr)
	{
		body->SetScale(s.x,s.y,s.z);
	}
}

void BoxCollider::SetPos(float3 p)
{
	if (body != nullptr)
	{
		body->SetPos(p.x, p.y, p.z);
	}
}

float* BoxCollider::GetTransform()
{
	return body->GetTransformPtr();
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