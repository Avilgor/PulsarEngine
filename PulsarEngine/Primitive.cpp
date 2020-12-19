#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Primitive.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"


Primitive::Primitive(PrimitiveTypes t) : color(White), wire(false), axis(false), type(t)
{
}

PrimitiveTypes Primitive::GetType()
{
	return type;
}


/*void Primitive::SetPos(float3 pos)
{
	transform.translate(x, y, z);
}

void Primitive::SetRotation(float3 rot)
{

}

void Primitive::SetScale(float3 scale)
{
	transform.scale(x, y, z);
}*/

// CUBE ============================================
CubePrimitive::CubePrimitive() : Primitive(Primitive_Cube), size(1.0f, 1.0f, 1.0f)
{}

CubePrimitive::CubePrimitive(float sizeX, float sizeY, float sizeZ) : Primitive(Primitive_Cube), size(sizeX, sizeY, sizeZ)
{
	type = PrimitiveTypes::Primitive_Cube;
}

void CubePrimitive::Render(float4x4 transformMatrix)
{	
	// Render a cube
	glPushMatrix();
	glMultMatrixf((float*)&transformMatrix);
	glBegin(GL_QUADS);
	// Top face
	glColor3f(0.0f, 1.0f, 0.0f);  // Green
	glVertex3f(1.0f, 1.0f, -1.0f);  // Top-right of top face
	glVertex3f(-1.0f, 1.0f, -1.0f);  // Top-left of top face
	glVertex3f(-1.0f, 1.0f, 1.0f);  // Bottom-left of top face
	glVertex3f(1.0f, 1.0f, 1.0f);  // Bottom-right of top face

	// Bottom face
	glColor3f(0.0f, 1.0f, 0.0f);  // Green
	glVertex3f(1.0f, -1.0f, -1.0f); // Top-right of bottom face
	glVertex3f(-1.0f, -1.0f, -1.0f); // Top-left of bottom face
	glVertex3f(-1.0f, -1.0f, 1.0f); // Bottom-left of bottom face
	glVertex3f(1.0f, -1.0f, 1.0f); // Bottom-right of bottom face

	// Front face
	glColor3f(0.0f, 1.0f, 0.0f);  // Green
	glVertex3f(1.0f, 1.0f, 1.0f);  // Top-Right of front face
	glVertex3f(-1.0f, 1.0f, 1.0f);  // Top-left of front face
	glVertex3f(-1.0f, -1.0f, 1.0f);  // Bottom-left of front face
	glVertex3f(1.0f, -1.0f, 1.0f);  // Bottom-right of front face

	// Back face
	glColor3f(0.0f, 1.0f, 0.0f);  // Green
	glVertex3f(1.0f, -1.0f, -1.0f); // Bottom-Left of back face
	glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom-Right of back face
	glVertex3f(-1.0f, 1.0f, -1.0f); // Top-Right of back face
	glVertex3f(1.0f, 1.0f, -1.0f); // Top-Left of back face

	// Left face
	glColor3f(0.0f, 1.0f, 0.0f);  // Green
	glVertex3f(-1.0f, 1.0f, 1.0f);  // Top-Right of left face
	glVertex3f(-1.0f, 1.0f, -1.0f);  // Top-Left of left face
	glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom-Left of left face
	glVertex3f(-1.0f, -1.0f, 1.0f);  // Bottom-Right of left face

	// Right face
	glColor3f(0.0f, 1.0f, 0.0f);  // Green
	glVertex3f(1.0f, 1.0f, 1.0f);  // Top-Right of left face
	glVertex3f(1.0f, 1.0f, -1.0f);  // Top-Left of left face
	glVertex3f(1.0f, -1.0f, -1.0f);  // Bottom-Left of left face
	glVertex3f(1.0f, -1.0f, 1.0f);  // Bottom-Right of left face

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnd();
	glPopMatrix();
}

// SPHERE ============================================
SpherePrimitive::SpherePrimitive() : Primitive(Primitive_Sphere), radius(1.0f)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

SpherePrimitive::SpherePrimitive(float radius) : Primitive(Primitive_Sphere), radius(radius)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

void SpherePrimitive::Render(float4x4 transformMatrix)
{
	glPushMatrix();
	glMultMatrixf((float*)&transformMatrix);

	glEnd();
	glPopMatrix();
}