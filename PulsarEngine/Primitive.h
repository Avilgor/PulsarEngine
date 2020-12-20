#pragma once
#include "Color.h"
#include "PhysBody3D.h"
#include "MathGeoLib/include/MathGeoLib.h"

#pragma comment (lib, "MathGeoLib/libx86/MathGeoLib.lib") 

enum PrimitiveTypes
{
	Primitive_Cube,
	Primitive_Sphere
};

class Primitive
{
public:

	Primitive(PrimitiveTypes t);

	virtual void Render(float4x4 transformMatrix) {}
	/*void SetPos(float3 position);
	void SetRotation(float3 euler);
	void SetScale(float3 scale);*/
	PrimitiveTypes GetType();

public:
	
	Color color;
	bool axis,wire;
	PhysBody3D* body;

protected:
	PrimitiveTypes type;
};

// ============================================
class CubePrimitive : public Primitive
{
public:
	CubePrimitive();
	CubePrimitive(float sizeX, float sizeY, float sizeZ);
	void Render(float4x4 transformMatrix);
public:
	float3 size;
};

// ============================================
class SpherePrimitive : public Primitive
{
public:
	SpherePrimitive();
	SpherePrimitive(float radius);
	void Render(float4x4 transformMatrix);
public:
	float radius;

private:
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> texcoords;
	std::vector<GLushort> indices;
	int rings = 10;
	int sectors = 10;
};