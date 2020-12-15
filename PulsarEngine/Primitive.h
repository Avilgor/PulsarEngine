#pragma once
//#include "glmath.h"
#include "Color.h"
#include "PhysBody3D.h"
#include "MathGeoLib/include/MathGeoLib.h"

#pragma comment (lib, "MathGeoLib/libx86/MathGeoLib.lib") 

enum PrimitiveTypes
{
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder
};

class Primitive
{
public:

	Primitive();

	virtual void Render() const;
	virtual void InnerRender() const;
	void SetPos(float x, float y, float z);
	void SetRotation(float angle, const float3 &u);
	void Scale(float x, float y, float z);
	PrimitiveTypes GetType() const;

public:
	
	Color color;
	float4x4 transform;
	bool axis,wire;
	PhysBody3D* body;
	float3 scale;

protected:
	PrimitiveTypes type;
};

// ============================================
class PhysCube : public Primitive
{
public:
	PhysCube();
	PhysCube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	float3 size;
};

// ============================================
class PhysSphere : public Primitive
{
public:
	PhysSphere();
	PhysSphere(float radius);
	void InnerRender() const;
public:
	float radius;
};

// ============================================
class PhysCylinder : public Primitive
{
public:
	PhysCylinder();
	PhysCylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class PhysLine : public Primitive
{
public:
	PhysLine();
	PhysLine(float x, float y, float z);
	void InnerRender() const;
public:
	float3 origin;
	float3 destination;
};

// ============================================
class PhysPlane : public Primitive
{
public:
	PhysPlane();
	PhysPlane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	float3 normal;
	float constant;
};