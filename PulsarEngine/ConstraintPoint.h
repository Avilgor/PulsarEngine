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

	void SetActive(bool val);
	bool IsActive();

private:
	btTypedConstraint* constraint;

public:
	PhysBody3D* bodyA = nullptr;
	PhysBody3D* bodyB = nullptr;
	float3 anchorA;
	float3 anchorB;
};

#endif //__ConstraintPoint_H__