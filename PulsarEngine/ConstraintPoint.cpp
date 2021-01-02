#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "ConstraintPoint.h"
#include "MathGeoLib/include/MathGeoLib.h"

ConstraintPoint::ConstraintPoint(GameObject* parent) : Component(parent, CONSTRAINT_POINT_COMP)
{	
	pointConstraint = this;
	needtoload = false;
	anchorA = float3::zero;
	anchorB = float3::zero;
	App->physics->AddConstraint(component);
}


ConstraintPoint::~ConstraintPoint()
{
	App->physics->RemoveConstraint(UUID);
}

void ConstraintPoint::CreateConstraint()
{
	if (bodyA != nullptr && bodyB != nullptr)
	{
		float3 posA = bodyA->GetPos() + anchorA;
		float3 posB = bodyB->GetPos() + anchorB;
		constraint = App->physics->AddConstraintPoint(*bodyA,*bodyB, vec3(posA.x, posA.y, posA.z), vec3(posB.x, posB.y, posB.z),UUID);
	}
}

void ConstraintPoint::UpdateComponent()
{
	if (bodyAComp == nullptr) SetBodyA(gameobject->GetColliderComp());
	if (bodyAComp != nullptr && bodyAComp->gameobject->GetColliderComp() == nullptr)
	{
		bodyAComp = nullptr;
		bodyA = nullptr;
	}

	if (bodyBComp != nullptr && (bodyAComp->gameobject->toDelete || bodyBComp->gameobject->GetColliderComp() == nullptr))
	{
		bodyBComp = nullptr;
		bodyB = nullptr;		
	}

	if (needtoload)
	{
		SetBodyA(gameobject->GetColliderComp());
		if (loadB_id.compare("0") != 0)
		{
			bodyBComp = App->physics->GetColliderByUUID(loadB_id);
			bodyB = App->physics->GetBodyByUUID(loadB_id);
		}
		needtoload = false;		
	}
}


void ConstraintPoint::ClearBodyB()
{
	if (bodyBComp != nullptr) bodyBComp = nullptr;
	if (bodyB != nullptr) bodyB = nullptr;
}

void ConstraintPoint::SetActive(bool val)
{
	constraint->setEnabled(val);
}

void ConstraintPoint::DeleteComponent()
{
	App->physics->RemoveConstraint(UUID);
}

bool ConstraintPoint::IsActive()
{
	return constraint->isEnabled();
}

void ConstraintPoint::SetBodyA(Component* comp)
{	
	if (comp != nullptr)
	{	
		if (bodyBComp != nullptr)
		{
			if (comp->UUID.compare(bodyBComp->UUID) != 0)
			{
				bodyAComp = comp;
				bodyA = App->physics->GetBodyByUUID(comp->UUID);
			}
		}
		else
		{
			bodyAComp = comp;
			bodyA = App->physics->GetBodyByUUID(comp->UUID);
		}
	}
}

void ConstraintPoint::SetOffsetA(float3 val)
{ 
	anchorA = val;
}

void ConstraintPoint::SetOffsetB(float3 val)
{ 
	anchorB = val;
}

void ConstraintPoint::SetBodyB(Component* comp)
{
	if (comp != nullptr)
	{
		if (bodyAComp != nullptr)
		{
			if (comp->UUID.compare(bodyAComp->UUID) != 0)
			{
				bodyBComp = comp;
				bodyB = App->physics->GetBodyByUUID(comp->UUID);
			}
		}
		else
		{
			bodyBComp = comp;
			bodyB = App->physics->GetBodyByUUID(comp->UUID);
		}
	}
}

void ConstraintPoint::SaveComponent(JSonHandler* file)
{
	JSonHandler node = file->InsertNodeArray("Components");
	node.SaveNum("CompType", (double)compType);
	node.SaveString("UUID", UUID.c_str());
	node.SaveBool("Active",active);

	if(bodyBComp!= nullptr) node.SaveString("BodyB",bodyBComp->UUID.c_str());
	else  node.SaveString("BodyB", "0");
	//AnchorA
	node.SaveNum("AnchorAx", anchorA.x);
	node.SaveNum("AnchorAy", anchorA.y);
	node.SaveNum("AnchorAz", anchorA.z);
	//AnchorB
	node.SaveNum("AnchorBx", anchorA.x);
	node.SaveNum("AnchorBy", anchorB.y);
	node.SaveNum("AnchorBz", anchorB.z);
}

void ConstraintPoint::LoadComponent(JSonHandler* file)
{
	needtoload = true;
	UUID = file->GetString("UUID");
	active = file->GetBool("Active");
	loadB_id = file->GetString("BodyB");
	//AnchorA
	anchorA.x = file->GetNum("AnchorAx");
	anchorA.y = file->GetNum("AnchorAy");
	anchorA.z = file->GetNum("AnchorAz");
	//AnchorB
	anchorB.x = file->GetNum("AnchorBx");
	anchorB.y = file->GetNum("AnchorBy");
	anchorB.z = file->GetNum("AnchorBz");
}