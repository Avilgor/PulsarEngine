#include "Globals.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include "Material.h"
#include <vector>

Mesh::Mesh(GameObject* parent) : Component(parent, MESH_COMP)
{
	/*VAO = 0;
	idVertex = 0;
	idIndex = 0;
	idText = 0;
	idNormals = 0;
	indexSize = 0;
	normalsSize = 0;
	verticesSize = 0;
	textSize = 0;*/
	path = "";
	component->mesh = this;
}

Mesh::~Mesh()
{}

void Mesh::UpdateComponent()
{
	if(!meshes.empty()) Render();
}

void Mesh::DeleteComponent()
{
	if (!meshes.empty())
	{
		for (std::vector<MeshInfo>::iterator it = meshes.begin(); it != meshes.end(); ++it)
		{
			(*it).Clean();
		}
		meshes.clear();
	}
	/*delete indicesArray;
	delete verticesArray;
	delete normalsArray;
	delete texturesArray;*/
	delete this;
}

bool Mesh::LoadImportedMesh()
{
	bool ret = true;
	GenerateBuffers();
	return ret;
}

void Mesh::AddMesh(MeshInfo mesh)
{
	meshes.push_back(mesh);
}

void Mesh::RemoveMesh(int index)
{
	if (!meshes.empty() && index < meshes.size())
	{
		meshes[index].Clean();
		meshes.erase(meshes.begin() + index);
	}
}

void Mesh::ReplaceMesh(MeshInfo mesh, int index)
{
	if (!meshes.empty() && index < meshes.size())
	{
		meshes[index].Clean();
		meshes[index] = mesh;
	}
}

void Mesh::GenerateBuffers()
{
	if (!meshes.empty())
	{
		for (std::vector<MeshInfo>::iterator it = meshes.begin(); it != meshes.end(); ++it)
		{
			if ((*it).verticesSize > 0)
			{
				glGenVertexArrays(1, &(*it).VAO);
				glBindVertexArray((*it).VAO);

				//VertexBuffer
				glGenBuffers(1, (GLuint*)&((*it).idVertex));
				glBindBuffer(GL_ARRAY_BUFFER, (*it).idVertex);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (*it).verticesSize * 3, (*it).verticesArray, GL_STATIC_DRAW);

				//Index buffer
				if ((*it).indexSize > 0)
				{
					glGenBuffers(1, (GLuint*)&((*it).idIndex));
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*it).idIndex);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * (*it).indexSize, (*it).indicesArray, GL_STATIC_DRAW);
				}

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);

				//Textures buffer
				if ((*it).textSize > 0)
				{
					glGenBuffers(1, &(*it).idText);
					glBindBuffer(GL_ARRAY_BUFFER, (*it).idText);
					glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (*it).textSize * 2, (*it).texturesArray, GL_STATIC_DRAW);

					glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
					glEnableVertexAttribArray(1);
				}

				//Normals buffer
				if ((*it).normalsSize > 0)
				{
					glGenBuffers(1, &(*it).idNormals);
					glBindBuffer(GL_ARRAY_BUFFER, (*it).idNormals);
					glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (*it).normalsSize * 3, (*it).normalsArray, GL_STATIC_DRAW);

					glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
					glEnableVertexAttribArray(2);
				}

				glBindVertexArray(0);
			}
		}
	}
}

void Mesh::Render()
{
	for (int i = 0;i<meshes.size();i++)
	{
		/*glPushMatrix();
		glMultMatrixf((float*)&gameobject->transform->GetTransform());
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, idVertex);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndex);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, NULL);
		glDisableClientState(GL_VERTEX_ARRAY);
		glPopMatrix();*/
		//if (meshes[i].material != nullptr)
		//{
			//lBindTexture(GL_TEXTURE_2D, idText);
		//}
		glPushMatrix();
		//glMatrixMode(GL_MODELVIEW);
		glMultMatrixf((float*)&gameobject->transform->GetTransformT());
		//glUniformMatrix4fv(0, 1, GL_FALSE, gameobject->transform->GetTransform().ptr());
		glEnableClientState(GL_VERTEX_ARRAY);

	
		//glBindVertexArray(VAO);	
		glBindBuffer(GL_ARRAY_BUFFER, meshes[i].idVertex);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i].idIndex);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glDrawElements(GL_TRIANGLES, meshes[i].indexSize, GL_UNSIGNED_INT, NULL);
	
	
		//glUseProgram(0);
		//glFrontFace(GL_CCW);
		//glBindVertexArray(0);
		glDisableClientState(GL_VERTEX_ARRAY);
		glPopMatrix();
	}
}

void Mesh::CreateCube(float x, float y, float z)
{
	MeshInfo mesh;
	mesh.name = "Cube mesh";
	GLuint index[] = { 0,1,2, 0,2,3, 1,4,5, 1,5,2, 6,0,3, 6,3,7, 3,2,5, 3,5,7, 6,4,1, 6,1,0, 4,6,7, 4,7,5 };
	GLfloat vertices[] = { 0.0f + x,0.0f + y,0.0f + z, 1.0f + x,0.0f + y,0.0f + z, 1.0f + x,1.0f + y,0.0f + z, 0.0f + x,1.0f + y,0.0f + z,
		1.0f + x,0.0f + y,-1.0f + z, 1.0f + x,1.0f + y,-1.0f + z, 0.0f + x,0.0f + y,-1.0f + z, 0.0f + x,1.0f + y,-1.0f + z };

	mesh.verticesSize = (sizeof(vertices) / sizeof(*vertices)) / 3;
	mesh.verticesArray = new float[mesh.verticesSize * 3];
	memcpy(mesh.verticesArray, vertices, sizeof(float) * mesh.verticesSize * 3);

	mesh.indexSize = sizeof(index) / sizeof(*index);
	mesh.indicesArray = new uint[mesh.indexSize];
	memcpy(mesh.indicesArray, index, sizeof(uint) * mesh.indexSize);
	meshes.push_back(mesh);
	GenerateBuffers();
	
}

void Mesh::CreatePyramid(float x, float y, float z)
{
	MeshInfo mesh;
	mesh.name = "Piramid mesh";
	GLuint index[] = { 3,2,1, 3,1,0, 0,1,4, 1,2,4, 2,3,4, 3,0,4 };
	GLfloat vertices[] = { 0.0f + x,0.0f + y,0.0f + z, 1.0f + x,0.0f + y,0.0f + z, 1.0f + x,0.0f + y,-1.0f + z, 0.0f + x,0.0f + y,-1.0f + z, 0.5f + x,1.0f + y,-0.5f + z };
	mesh.verticesSize = (sizeof(vertices) / sizeof(*vertices)) / 3;
	mesh.verticesArray = new float[mesh.verticesSize * 3];
	memcpy(mesh.verticesArray, vertices, sizeof(float) * mesh.verticesSize * 3);

	mesh.indexSize = sizeof(index) / sizeof(*index);
	mesh.indicesArray = new uint[mesh.indexSize];
	memcpy(mesh.indicesArray, index, sizeof(uint) * mesh.indexSize);
	meshes.push_back(mesh);
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
	MeshInfo mesh;
	mesh.name = "Plane mesh";
	GLuint index[] = { 0,1,2,	0,2,3 };
	GLfloat vertices[] = { (size/2) + x,0.0f + y,(size/2) + z,	(size/2) + x,0.0f + y,(size/2) + z, (size/2) + x,0.0f + y,(-size/2) + z,	(-size/2) + x,0.0f + y,(-size/2)+ z };
	mesh.verticesSize = (sizeof(vertices) / sizeof(*vertices)) / 3;
	mesh.verticesArray = new float[mesh.verticesSize * 3];
	memcpy(mesh.verticesArray, vertices, sizeof(float) * mesh.verticesSize * 3);

	mesh.indexSize = sizeof(index) / sizeof(*index);
	mesh.indicesArray = new uint[mesh.indexSize];
	memcpy(mesh.indicesArray, index, sizeof(uint) * mesh.indexSize);
	meshes.push_back(mesh);
	GenerateBuffers();
}