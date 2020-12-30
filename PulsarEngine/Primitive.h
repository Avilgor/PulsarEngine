#pragma once
#include "Color.h"
#include "PhysBody3D.h"
#include "MathGeoLib/include/MathGeoLib.h"

#pragma comment (lib, "MathGeoLib/libx86/MathGeoLib.lib") 

enum PrimitiveTypes
{
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder,
	Primitive_Point,
	Primitive_Line
};

class Primitive
{
public:

	Primitive();

	virtual void Render();
	virtual void InnerRender();
	void SetPos(float x, float y, float z);
	void SetRotation(float angle, const vec3& u);
	void Scale(float x, float y, float z);
	PrimitiveTypes GetType();

public:
	
	Color color;
	bool axis,wire;
	PhysBody3D* body;
	mat4x4 transform;

protected:
	PrimitiveTypes type;
};

// ============================================
class CubePrimitive : public Primitive
{
public:
	CubePrimitive();
	CubePrimitive(float sizeX, float sizeY, float sizeZ);
	void InnerRender();
public:
	float3 size;
};

// ============================================
class SpherePrimitive : public Primitive
{
public:
	SpherePrimitive();
	SpherePrimitive(float radius);
	void InnerRender();
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

// ============================================
class CylinderPrimitive : public Primitive
{
public:
	CylinderPrimitive();
	CylinderPrimitive(float radius, float height);
	void InnerRender();
public:
	float radius;
	float height;
};


// ============================================
class LinePrimitive : public Primitive
{
public:
	LinePrimitive();
	LinePrimitive(float x, float y, float z);
	void InnerRender();
public:
	vec3 origin;
	vec3 destination;
};