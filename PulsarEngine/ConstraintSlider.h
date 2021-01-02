#ifndef __ConstraintSlider_H__
#define __ConstraintSlider_H__

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ConstraintSlider : public Component
{
public:

	ConstraintSlider(GameObject* parent);
	~ConstraintSlider();

	void CreateConstraint();
	void UpdateComponent();
	void DeleteComponent();
	void SaveComponent(JSonHandler* file);
	void LoadComponent(JSonHandler* file);
	void SetBodyA(Component* comp);
	void SetBodyB(Component* comp);
	//float3 GetOffsetA() { return anchorA; }
	//float3 GetOffsetB() { return anchorB; }
	//void SetOffsetA(float3 val);
	//void SetOffsetB(float3 val);
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

#endif //__ConstraintSlider_H__