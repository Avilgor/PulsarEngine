#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "ConstraintSlider.h"
#include "MathGeoLib/include/MathGeoLib.h"

ConstraintSlider::ConstraintSlider(GameObject* parent) : Component(parent, CONSTRAINT_SLIDER_COMP)
{
	sliderConstraint = this;
	needtoload = false;
	anchorA = float3::zero;
	anchorB = float3::zero;
	App->physics->AddConstraint(component);
}


ConstraintSlider::~ConstraintSlider()
{
	if (constraint != nullptr)
	{
		App->physics->RemoveConstraint(UUID);
	}
}

void ConstraintSlider::CreateConstraint()
{
	if (bodyA != nullptr && bodyB != nullptr)
	{
		constraint = App->physics->AddConstraintSlider(*bodyA, *bodyB, bodyA->body->getWorldTransform(), bodyB->body->getWorldTransform(), UUID);
	}
}

void ConstraintSlider::UpdateComponent()
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

void ConstraintSlider::ClearBodyB()
{
	if (bodyBComp != nullptr) bodyBComp = nullptr;
	if (bodyB != nullptr) bodyB = nullptr;
}

void ConstraintSlider::SetActive(bool val)
{
	constraint->setEnabled(val);
}

void ConstraintSlider::DeleteComponent()
{
	App->physics->RemoveConstraint(UUID);	
}

bool ConstraintSlider::IsActive()
{
	return constraint->isEnabled();
}

void ConstraintSlider::SetBodyA(Component* comp)
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
/*
void ConstraintSlider::SetOffsetA(float3 val)
{
	anchorA = val;
	UpdateConstraint();
}

void ConstraintSlider::SetOffsetB(float3 val)
{
	anchorB = val;
	UpdateConstraint();
}
*/
void ConstraintSlider::SetBodyB(Component* comp)
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

void ConstraintSlider::SaveComponent(JSonHandler* file)
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

void ConstraintSlider::LoadComponent(JSonHandler* file)
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