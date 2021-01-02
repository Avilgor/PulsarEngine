#ifndef __ConstraintCone_H__
#define __ConstraintCone_H__

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ConstraintCone : public Component
{
public:

	ConstraintCone(GameObject* parent);
	~ConstraintCone();

	void CreateConstraint();
	void UpdateComponent();
	void DeleteComponent();
	void SaveComponent(JSonHandler* file);
	void LoadComponent(JSonHandler* file);
	void SetBodyA(Component* comp);
	void SetBodyB(Component* comp);
	/*float3 GetOffsetA() { return anchorA; }
	float3 GetOffsetB() { return anchorB; }
	void SetOffsetA(float3 val);
	void SetOffsetB(float3 val);*/
	void ClearBodyB();

	void SetActive(bool val);
	bool IsActive();

private:

	std::string loadB_id;
	bool needtoload;

public:
	btTypedConstraint* constraint = nullptr;
	Component* bodyAComp = nullptr;
	Component* bodyBComp = nullptr;
	PhysBody3D* bodyA = nullptr;
	PhysBody3D* bodyB = nullptr;
	float3 anchorA;
	float3 anchorB;
};

#endif //__ConstraintCone_H__