#ifndef __Transform_H__
#define __Transform_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

#pragma comment (lib, "MathGeoLib/libx86/MathGeoLib.lib") 

class JSonHandler;

class Transform : public Component
{
public:

	Transform(GameObject* parent);
	Transform(GameObject* parent, float3 position, Quat rotation = Quat::identity, float3 scale = float3::one);
	Transform(GameObject* parent, float3 position, float3 rotation, float3 scale);
	Transform(GameObject* parent, float4x4 transform);
	~Transform();

	void UpdateComponent();
	void DeleteComponent();
	void ResetTransform();
	void UpdateEuler();
	float4x4 GetTransform() { return transform; }
	float4x4 GetTransformTransposed();
	float3 GetLocalPosition() { return position; }
	float3 GetGlobalPosition();
	float3 GetLocalRotation() { return eulerRotation; }
	float3 GetGlobalRotation();
	float3 GetLocalScale() { return scale; }
	float3 GetGlobalScale();
	float3 GetEulerRotation() { return eulerRotation; }
	Quat GetQuaternionRotation() { return quaternionRotation; }
	float3 GetScale() { return scale; }
	float3 RotationToEuler() { return eulerRotation; }
	Quat RotationToQuaternion() { return quaternionRotation; }
	float4x4 GetGlobalTransform() { return transformGlobal; }
	float4x4 GetGlobalTransformTransposed();

	void Rotate(float3 rot);
	void SetPosition(float3 pos);
	void SetScale(float3 scale);
	void SetQuatRotation(Quat rot);
	void SetEulerRotation(float3 degrees);
	void SetGlobalTransform();
	void SaveComponent(JSonHandler* file);
	void LoadComponent(JSonHandler* file);

private:
	void UpdateTRS();
	void UpdateLocal();	
	void UpdateQuaternion();

public:
	bool updateTransform;

private:
	float4x4 transform;
	float4x4 transformGlobal;
	float3 position;
	float3 scale;
	Quat quaternionRotation;
	float3 eulerRotation;	

	float3 loadPos;
	float3 loadRot;
	float3 loadScale;
	bool loadedTransform;
};

#endif //__Transform_H__