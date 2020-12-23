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
	created = false;
}


ConstraintPoint::~ConstraintPoint()
{
	if (constraint != nullptr)
	{
		App->physics->RemoveConstraint(constraint,UUID);
		delete constraint;
	}
}

void ConstraintPoint::CreateConstraint()
{
	if (bodyA != nullptr && bodyB != nullptr)
	{
		if (constraint != nullptr)
		{
			App->physics->RemoveConstraint(UUID);
			delete constraint;
		}
		constraint = App->physics->AddConstraintPoint(*bodyA,*bodyB, vec3(anchorA.x, anchorA.y, anchorA.z), vec3(anchorB.x, anchorB.y, anchorB.z),UUID);
		//LOG("Constraint point created");
		created = true;
	}
}

void ConstraintPoint::UpdateComponent()
{
	if (bodyAComp != nullptr && bodyAComp->gameobject->toDelete == true)
	{
		if (constraint != nullptr)
		{
			App->physics->RemoveConstraint(UUID);
			delete constraint;
			constraint = nullptr;
		}
		bodyAComp = nullptr;
		bodyA = nullptr;
	}

	if (bodyBComp != nullptr && bodyBComp->gameobject->toDelete == true)
	{
		if (constraint != nullptr)
		{
			App->physics->RemoveConstraint(UUID);
			delete constraint;
			constraint = nullptr;
		}
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
		if (bodyA != nullptr && bodyB != nullptr) CreateConstraint();
		needtoload = false;		
	}

	if (!created && !needtoload) CreateConstraint();
}

void ConstraintPoint::UpdateConstraint()
{
	if (constraint != nullptr)
	{
		App->physics->RemoveConstraint(UUID);
		delete constraint;
		constraint = nullptr;
	}

	if (bodyA != nullptr && bodyB != nullptr)
	{
		constraint = App->physics->AddConstraintPoint(*bodyA, *bodyB, vec3(anchorA.x, anchorA.y, anchorA.z), vec3(anchorB.x, anchorB.y, anchorB.z), UUID);
		created = true;
	}
}

void ConstraintPoint::ClearBodyB()
{
	if (bodyBComp != nullptr) bodyBComp = nullptr;
	if (bodyB != nullptr) bodyB = nullptr;
	if (constraint != nullptr)
	{
		App->physics->RemoveConstraint(UUID);
		delete constraint;
		constraint = nullptr;
	}
}

void ConstraintPoint::SetActive(bool val)
{
	constraint->setEnabled(val);
}

void ConstraintPoint::DeleteComponent()
{
	if (constraint != nullptr)
	{
		App->physics->RemoveConstraint(constraint, UUID);
		delete constraint;
		constraint = nullptr;
	}
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
				created = false;
			}
		}
		else
		{
			bodyAComp = comp;
			bodyA = App->physics->GetBodyByUUID(comp->UUID);
			created = false;
		}
	}
}

void ConstraintPoint::SetOffsetA(float3 val)
{ 
	anchorA = val;
	UpdateConstraint();
}

void ConstraintPoint::SetOffsetB(float3 val)
{ 
	anchorB = val;
	UpdateConstraint();
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
				created = false;
			}
		}
		else
		{
			bodyBComp = comp;
			bodyB = App->physics->GetBodyByUUID(comp->UUID);
			created = false;
		}
	}
}

void ConstraintPoint::SaveComponent(JSonHandler* file)
{
	JSonHandler node = file->InsertNodeArray("Components");
	node.SaveNum("CompType", (double)compType);
	node.SaveString("UUID", UUID.c_str());
	node.SaveBool("Active",active);
	//if (bodyAComp != nullptr) node.SaveString("BodyA",bodyAComp->UUID.c_str());
	//else node.SaveString("BodyA", "0");
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
	//loadA_id = file->GetString("BodyA");
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