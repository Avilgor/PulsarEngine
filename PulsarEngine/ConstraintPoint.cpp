#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "ConstraintPoint.h"
#include "MathGeoLib/include/MathGeoLib.h"

ConstraintPoint::ConstraintPoint(GameObject* parent) : Component(parent, CONSTRAINT_POINT_COMP)
{
	
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

}

void ConstraintPoint::UpdateComponent()
{
	
}

void ConstraintPoint::SetActive(bool val)
{
	constraint->setEnabled(val);
}

void ConstraintPoint::DeleteComponent()
{
	App->physics->RemoveConstraint(constraint, UUID);
	delete constraint;
	delete this;
}

bool ConstraintPoint::IsActive()
{
	return constraint->isEnabled();
}


void ConstraintPoint::SaveComponent(JSonHandler* file)
{
	JSonHandler node = file->InsertNodeArray("Components");
	node.SaveNum("CompType", (double)compType);
	node.SaveString("UUID", UUID.c_str());
	
}

void ConstraintPoint::LoadComponent(JSonHandler* file)
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