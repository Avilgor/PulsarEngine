#ifndef __ConstraintPoint_H__
#define __ConstraintPoint_H__

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ConstraintPoint : public Component
{
public:

	ConstraintPoint(GameObject* parent);
	~ConstraintPoint();

	void CreateConstraint();
	void UpdateComponent();
	void DeleteComponent();
	void SaveComponent(JSonHandler* file);
	void LoadComponent(JSonHandler* file);
	void SetBodyA(Component* comp);
	void SetBodyB(Component* comp);
	float3 GetOffsetA() { return anchorA; }
	float3 GetOffsetB() { return anchorB; }
	void SetOffsetA(float3 val);
	void SetOffsetB(float3 val);
	void ClearBodyB();
	void UpdateConstraint();

	void SetActive(bool val);
	bool IsActive();

private:
	btTypedConstraint* constraint = nullptr;
	std::string loadB_id;
	bool needtoload;
	bool created;

public:
	Component* bodyAComp = nullptr;
	Component* bodyBComp = nullptr;
	PhysBody3D* bodyA = nullptr;
	PhysBody3D* bodyB = nullptr;
	float3 anchorA;
	float3 anchorB;
};

#endif //__ConstraintPoint_H__