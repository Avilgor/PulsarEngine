#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Primitive.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"


Primitive::Primitive() : color(White), wire(false), axis(false), type(Primitive_Point)
{
}

PrimitiveTypes Primitive::GetType()
{
	return type;
}

void Primitive::Render()
{
	glPushMatrix();
	glMultMatrixf(transform.M);

	/*if (axis == true)
	{
		// Draw Axis Grid
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
		glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
		glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
		glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

		glEnd();

		glLineWidth(1.0f);
	}*/

	glColor3f(color.r, color.g, color.b);

	if (wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	InnerRender();
	glColor3f(1.0f, 1.0f, 1.0f);
	glPopMatrix();
}

void Primitive::InnerRender()
{
	glPointSize(5.0f);

	glBegin(GL_POINTS);

	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();

	glPointSize(1.0f);
}


void Primitive::SetPos(float x, float y, float z)
{
	transform.translate(x, y, z);
}

void Primitive::SetRotation(float angle, const vec3& u)
{
	transform.rotate(angle, u);
}

void Primitive::Scale(float x, float y, float z)
{
	transform.scale(x, y, z);
}

// CUBE ============================================
CubePrimitive::CubePrimitive() : Primitive(), size(1.0f, 1.0f, 1.0f)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

CubePrimitive::CubePrimitive(float sizeX, float sizeY, float sizeZ) : Primitive(), size(sizeX, sizeY, sizeZ)
{
	type = PrimitiveTypes::Primitive_Cube;
}

void CubePrimitive::InnerRender()
{	
	// Render a cube
	/*glBegin(GL_QUADS);
	// Top face
	glColor3f(0.0f, 1.0f, 0.0f);  // Green
	glVertex3f(size.x, size.y, -size.z);  // Top-right of top face
	glVertex3f(-size.x, size.y, -size.z);  // Top-left of top face
	glVertex3f(-size.x, size.y, size.z);  // Bottom-left of top face
	glVertex3f(size.x, size.y, size.z);  // Bottom-right of top face

	// Bottom face
	glColor3f(0.0f, 1.0f, 0.0f);  // Green
	glVertex3f(size.x, -size.y, -size.z); // Top-right of bottom face
	glVertex3f(-size.x, -size.y, -size.z); // Top-left of bottom face
	glVertex3f(-size.x, -size.y, size.z); // Bottom-left of bottom face
	glVertex3f(size.x, -size.y, size.z); // Bottom-right of bottom face

	// Front face
	glColor3f(0.0f, 1.0f, 0.0f);  // Green
	glVertex3f(size.x, size.y, size.z);  // Top-Right of front face
	glVertex3f(-size.x, size.y, size.z);  // Top-left of front face
	glVertex3f(-size.x, -size.y, size.z);  // Bottom-left of front face
	glVertex3f(size.x, -size.y, size.z);  // Bottom-right of front face

	// Back face
	glColor3f(0.0f, 1.0f, 0.0f);  // Green
	glVertex3f(size.x, -size.y, -size.z); // Bottom-Left of back face
	glVertex3f(-size.x, -size.y, -size.z); // Bottom-Right of back face
	glVertex3f(-size.x, size.y, -size.z); // Top-Right of back face
	glVertex3f(size.x, size.y, -size.z); // Top-Left of back face

	// Left face
	glColor3f(0.0f, 1.0f, 0.0f);  // Green
	glVertex3f(-size.x, size.y, size.z);  // Top-Right of left face
	glVertex3f(-size.x, size.y, -size.z);  // Top-Left of left face
	glVertex3f(-size.x, -size.y, -size.z);  // Bottom-Left of left face
	glVertex3f(-size.x, -size.y, size.z);  // Bottom-Right of left face

	// Right face
	glColor3f(0.0f, 1.0f, 0.0f);  // Green
	glVertex3f(size.x, size.y, size.z);  // Top-Right of left face
	glVertex3f(size.x, size.y, -size.z);  // Top-Left of left face
	glVertex3f(size.x, -size.y, -size.z);  // Bottom-Left of left face
	glVertex3f(size.x, -size.y, size.z);  // Bottom-Right of left face

	glEnd();*/
}

// SPHERE ============================================
SpherePrimitive::SpherePrimitive() : Primitive(), radius(1.0f)
{
	type = PrimitiveTypes::Primitive_Sphere;
	/*float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	int r, s;

	vertices.resize(rings * sectors * 3);
	normals.resize(rings * sectors * 3);
	texcoords.resize(rings * sectors * 2);
	std::vector<GLfloat>::iterator v = vertices.begin();
	std::vector<GLfloat>::iterator n = normals.begin();
	std::vector<GLfloat>::iterator t = texcoords.begin();
	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) 
	{
		float const y = sin(-M_PI_2 + M_PI * r * R);
		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

		*t++ = s * S;
		*t++ = r * R;

		*v++ = x * radius;
		*v++ = y * radius;
		*v++ = z * radius;

		*n++ = x;
		*n++ = y;
		*n++ = z;
	}

	indices.resize(rings * sectors * 4);
	std::vector<GLushort>::iterator i = indices.begin();
	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++)
	{
		*i++ = r * sectors + s;
		*i++ = r * sectors + (s + 1);
		*i++ = (r + 1) * sectors + (s + 1);
		*i++ = (r + 1) * sectors + s;
	}*/
}

SpherePrimitive::SpherePrimitive(float radius) : Primitive(), radius(radius)
{
	type = PrimitiveTypes::Primitive_Sphere;

	/*float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	int r, s;

	vertices.resize(rings * sectors * 3);
	normals.resize(rings * sectors * 3);
	texcoords.resize(rings * sectors * 2);
	std::vector<GLfloat>::iterator v = vertices.begin();
	std::vector<GLfloat>::iterator n = normals.begin();
	std::vector<GLfloat>::iterator t = texcoords.begin();
	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) 
	{
		float const y = sin(-M_PI_2 + M_PI * r * R);
		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

		*t++ = s * S;
		*t++ = r * R;

		*v++ = x * radius;
		*v++ = y * radius;
		*v++ = z * radius;

		*n++ = x;
		*n++ = y;
		*n++ = z;
	}

	indices.resize(rings * sectors * 4);
	std::vector<GLushort>::iterator i = indices.begin();
	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) 
	{
		*i++ = r * sectors + s;
		*i++ = r * sectors + (s + 1);
		*i++ = (r + 1) * sectors + (s + 1);
		*i++ = (r + 1) * sectors + s;
	}*/
}

void SpherePrimitive::InnerRender()
{
	/*glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glColor3f(0.0f, 1.0f, 0.0f);  // Green
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glNormalPointer(GL_FLOAT, 0, &normals[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);

	glEnd();*/
}

// LINE ==================================================
LinePrimitive::LinePrimitive() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
{
	type = PrimitiveTypes::Primitive_Line;
}

LinePrimitive::LinePrimitive(float x, float y, float z) : Primitive(), origin(0, 0, 0), destination(x, y, z)
{
	type = PrimitiveTypes::Primitive_Line;
}

void LinePrimitive::InnerRender()
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(destination.x, destination.y, destination.z);

	glEnd();
	glLineWidth(1.0f);
}