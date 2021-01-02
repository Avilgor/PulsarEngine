#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "ConstraintHinge.h"
#include "MathGeoLib/include/MathGeoLib.h"

ConstraintHinge::ConstraintHinge(GameObject* parent) : Component(parent, CONSTRAINT_HINGE_COMP)
{
	hingeConstraint = this;
	needtoload = false;
	anchorA = float3::zero;
	anchorB = float3::zero;
	axisA = float3::zero;
	axisB = float3::zero;
	axisA.y = 1.0f;
	axisB.y = 1.0f;
	App->physics->AddConstraint(component);
}


ConstraintHinge::~ConstraintHinge()
{
	App->physics->RemoveConstraint(UUID);
}

void ConstraintHinge::CreateConstraint()
{
	if (bodyA != nullptr && bodyB != nullptr)
	{
		float3 posA = bodyA->GetPos() + anchorA;
		float3 posB = bodyB->GetPos() + anchorB;
		constraint = App->physics->AddConstraintHinge(*bodyA, *bodyB, vec3(posA.x, posA.y, posA.z), vec3(posB.x, posB.y, posB.z),
			vec3(axisA.x, axisA.y, axisA.z), vec3(axisB.x, axisB.y, axisB.z), UUID);
	}
}

void ConstraintHinge::UpdateComponent()
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

void ConstraintHinge::ClearBodyB()
{
	if (bodyBComp != nullptr) bodyBComp = nullptr;
	if (bodyB != nullptr) bodyB = nullptr;
}

void ConstraintHinge::SetActive(bool val)
{
	constraint->setEnabled(val);
}

void ConstraintHinge::DeleteComponent()
{
	App->physics->RemoveConstraint(UUID);
}

bool ConstraintHinge::IsActive()
{
	return constraint->isEnabled();
}

void ConstraintHinge::SetBodyA(Component* comp)
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

void ConstraintHinge::SetOffsetA(float3 val)
{
	anchorA = val;
}

void ConstraintHinge::SetOffsetB(float3 val)
{
	anchorB = val;
}

void ConstraintHinge::SetAxisA(float3 val)
{
	axisA = val;
}

void ConstraintHinge::SetAxisB(float3 val)
{
	axisB = val;
}

void ConstraintHinge::SetBodyB(Component* comp)
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

void ConstraintHinge::SaveComponent(JSonHandler* file)
{
	JSonHandler node = file->InsertNodeArray("Components");
	node.SaveNum("CompType", (double)compType);
	node.SaveString("UUID", UUID.c_str());
	node.SaveBool("Active", active);

	if (bodyBComp != nullptr) node.SaveString("BodyB", bodyBComp->UUID.c_str());
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

void ConstraintHinge::LoadComponent(JSonHandler* file)
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