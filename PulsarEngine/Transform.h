#ifndef __Transform_H__
#define __Transform_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

#pragma comment (lib, "MathGeoLib/libx86/MathGeoLib.lib") 


class Transform : public Component
{
public:

	Transform(GameObject* parent);
	Transform(GameObject* parent, float3 position, Quat rotation = Quat::identity, float3 scale = float3::one);
	Transform(GameObject* parent, float4x4 transform);
	~Transform();

	void UpdateComponent();
	void DeleteComponent();
	void ResetTransform();
	void UpdateEuler();
	float4x4 GetTransform() { return transform; }
	float4x4 GetTransformT() { return transformT; }
	float3 GetPosition() { return position; }
	float3 GetGlobalPosition();
	float3 GetEulerRotation() { return eulerRotation; }
	Quat GetQuaternionRotation() { return quaternionRotation; }
	float3 GetScale() { return scale; }
	float3 RotationToEuler() { return eulerRotation; }
	Quat RotationToQuaternion() { return quaternionRotation; }
	float4x4 GetGlobalTransform() { return transformGlobal; }
	float4x4 GetGlobalTransformT() { return transformTGlobal; }

	void Rotate(float3 rot);
	void SetPosition(float3 pos);
	void SetScale(float3 scale);
	void SetQuatRotation(Quat rot);
	void SetEulerRotation(float3 degrees);
	void SetGlobalTransform(float4x4 transform);

private:
	void UpdateTRS();
	void UpdateLocal();
	

private:
	float4x4 transform;
	float4x4 transformT;
	float4x4 transformGlobal;
	float4x4 transformTGlobal;
	float3 position;
	float3 scale;
	Quat quaternionRotation;
	float3 eulerRotation;
};

#endif //__Transform_H__