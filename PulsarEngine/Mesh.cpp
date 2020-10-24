#include "Globals.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"

Mesh::Mesh(GameObject* parent) : Component(parent, MESH_COMP)
{
	VAO = 0;
	idVertex = 0;
	idIndex = 0;
	idText = 0;
	idNormals = 0;
	indexSize = 0;
	normalsSize = 0;
	verticesSize = 0;
	textSize = 0;
	component->mesh = this;
}

Mesh::~Mesh()
{}

void Mesh::UpdateComponent()
{
	Render();
}

void Mesh::DeleteComponent()
{
	delete indicesArray;
	delete verticesArray;
	delete normalsArray;
	delete texturesArray;
	delete this;
}

bool Mesh::LoadImportedMesh()
{
	bool ret = true;
	GenerateBuffers();
	return ret;
}

void Mesh::GenerateBuffers()
{
	if (verticesSize > 0)
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//VertexBuffer
		glGenBuffers(1, (GLuint*)&(idVertex));
		glBindBuffer(GL_ARRAY_BUFFER, idVertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesSize * 3, verticesArray, GL_STATIC_DRAW);

		//Index buffer
		if (indexSize > 0)
		{
			glGenBuffers(1, (GLuint*)&(idIndex));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndex);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indexSize, indicesArray, GL_STATIC_DRAW);
		}

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//Textures buffer
		if (textSize > 0)
		{
			glGenBuffers(1, &idText);
			glBindBuffer(GL_ARRAY_BUFFER, idText);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * textSize * 2, texturesArray, GL_STATIC_DRAW);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
		}

		//Normals buffer
		if (normalsSize > 0)
		{
			glGenBuffers(1, &idNormals);
			glBindBuffer(GL_ARRAY_BUFFER, idNormals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normalsSize * 3, normalsArray, GL_STATIC_DRAW);

			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(2);
		}

		glBindVertexArray(0);
	}
}

void Mesh::Render()
{
	glPushMatrix();
	glMultMatrixf((float*)&gameobject->transform->GetTransform());
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, idVertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}

void Mesh::CreateCube(float x, float y, float z)
{
	GLuint index[] = { 0,1,2, 0,2,3, 1,4,5, 1,5,2, 6,0,3, 6,3,7, 3,2,5, 3,5,7, 6,4,1, 6,1,0, 4,6,7, 4,7,5 };
	GLfloat vertices[] = { 0.0f + x,0.0f + y,0.0f + z, 1.0f + x,0.0f + y,0.0f + z, 1.0f + x,1.0f + y,0.0f + z, 0.0f + x,1.0f + y,0.0f + z,
		1.0f + x,0.0f + y,-1.0f + z, 1.0f + x,1.0f + y,-1.0f + z, 0.0f + x,0.0f + y,-1.0f + z, 0.0f + x,1.0f + y,-1.0f + z };

	verticesSize = (sizeof(vertices) / sizeof(*vertices)) / 3;
	verticesArray = new float[verticesSize * 3];
	memcpy(verticesArray, vertices, sizeof(float) * verticesSize * 3);

	indexSize = sizeof(index) / sizeof(*index);
	indicesArray = new uint[indexSize];
	memcpy(indicesArray, index, sizeof(uint) * indexSize);
	GenerateBuffers();
}

void Mesh::CreatePyramid(float x, float y, float z)
{
	GLuint index[] = { 3,2,1, 3,1,0, 0,1,4, 1,2,4, 2,3,4, 3,0,4 };
	GLfloat vertices[] = { 0.0f + x,0.0f + y,0.0f + z, 1.0f + x,0.0f + y,0.0f + z, 1.0f + x,0.0f + y,-1.0f + z, 0.0f + x,0.0f + y,-1.0f + z, 0.5f + x,1.0f + y,-0.5f + z };
	verticesSize = (sizeof(vertices) / sizeof(*vertices)) / 3;
	verticesArray = new float[verticesSize * 3];
	memcpy(verticesArray, vertices, sizeof(float) * verticesSize * 3);

	indexSize = sizeof(index) / sizeof(*index);
	indicesArray = new uint[indexSize];
	memcpy(indicesArray, index, sizeof(uint) * indexSize);
	GenerateBuffers();
}

void Mesh::CreateSphere(float radius, int rings, int sectors, float x, float y, float z)
{
	/*float radius = 1;
	int rings = 12;
	int sectors = 24;

	float const R = 1.0f / (float)(rings - 1);
	float const S = 1.0f / (float)(sectors - 1);
	int r, s;

	vertices.resize(rings * sectors * 3);
	std::vector<GLfloat>::iterator v = vertices.begin();

	for (r = 0; r < rings; r++)
	{
		for (s = 0; s < sectors; s++)
		{
			float const y = sin(-M_PI * 0.5f + M_PI * r * R);
			float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
			float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

			*v++ = x * radius;
			*v++ = y * radius;
			*v++ = z * radius;
		}
	}

	indices.resize(rings * sectors * 4);
	std::vector<GLushort>::iterator i = indices.begin();

	for (r = 0; r < rings - 1; r++)
	{
		for (s = 0; s < sectors - 1; s++)
		{
			*i++ = (r + 1) * sectors + s;
			*i++ = (r + 1) * sectors + (s + 1);
			*i++ = r * sectors + (s + 1);
			*i++ = r * sectors + s;
		}
	}*/
}

void Mesh::CreatePlane(float size,float x, float y, float z)
{
	GLuint index[] = { 0,1,2,	0,2,3 };
	GLfloat vertices[] = { (size/2) + x,0.0f + y,(size/2) + z,	(size/2) + x,0.0f + y,(size/2) + z, (size/2) + x,0.0f + y,(-size/2) + z,	(-size/2) + x,0.0f + y,(-size/2)+ z };
	verticesSize = (sizeof(vertices) / sizeof(*vertices)) / 3;
	verticesArray = new float[verticesSize * 3];
	memcpy(verticesArray, vertices, sizeof(float) * verticesSize * 3);

	indexSize = sizeof(index) / sizeof(*index);
	indicesArray = new uint[indexSize];
	memcpy(indicesArray, index, sizeof(uint) * indexSize);
	GenerateBuffers();
}