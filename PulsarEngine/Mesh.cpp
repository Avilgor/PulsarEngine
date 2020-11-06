#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "RES_Mesh.h"
#include "JSonHandler.h"
#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include "Material.h"
#include <vector>

Mesh::Mesh(GameObject* parent) : Component(parent, MESH_COMP)
{
	pathFBX = "";
	component->mesh = this;
	drawTexture = true;
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
		for (std::vector<RES_Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
		{			
			(*it)->Clean();
			delete (*it);
		}
		meshes.clear();
	}
	delete this;
}

void Mesh::SetMaterialByUUID(RES_Material* mat)
{
//	LOG("Setting meshes materials");
	if (!meshes.empty())
	{
		for (std::vector<RES_Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
		{
			if ((*it)->materialUUID.compare(mat->UUID) == 0) (*it)->SetMaterial(mat);
		}
	}
}

void Mesh::LoadMesh(RES_Mesh* mesh)
{
	char* buffer = nullptr;
	std::string path = MESHES_PATH;
	path.append(mesh->name);
	mesh->path = path;
	App->fileSystem->Load(path.c_str(), &buffer);
	App->fileSystem->LoadMesh(mesh, buffer);
	GenerateBuffers(mesh);
}

void Mesh::AddMesh(RES_Mesh* mesh)
{
	meshes.push_back(mesh);	
	LoadMesh(mesh);
}

void Mesh::RemoveMesh(int index)
{
	if (!meshes.empty() && index < meshes.size())
	{
		meshes[index]->Clean();
		delete meshes[index];
		meshes.erase(meshes.begin() + index);
	}
}

void Mesh::ReplaceMesh(RES_Mesh* mesh, int index)
{
	if (!meshes.empty() && index < meshes.size())
	{
		meshes[index]->Clean();
		delete meshes[index];
		meshes[index] = mesh;
	}
}

RES_Mesh* Mesh::GetMesh(int index)
{
	RES_Mesh* ret = new RES_Mesh();
	if (!meshes.empty() && index < meshes.size())
	{
		ret = meshes[index];
	}
	return ret;
}

void Mesh::SetMeshMaterial(RES_Material* mat, int index)
{
	if (!meshes.empty())
	{
		int i = 0;
		for (std::vector<RES_Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
		{
			if(i == index) (*it)->SetMaterial(mat);
			i++;
		}
	}
}

void Mesh::SetAllMeshesMaterial(RES_Material* mat)
{
	if (!meshes.empty())
	{
		for (std::vector<RES_Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
		{
			(*it)->SetMaterial(mat);
		}
	}
}

void Mesh::GenerateBuffers(RES_Mesh* mesh)
{
	if (mesh->verticesSize > 0)
	{
		glGenVertexArrays(1, &mesh->VAO);
		glBindVertexArray(mesh->VAO);

		//VertexBuffer
		glGenBuffers(1, (GLuint*)&(mesh->idVertex));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->idVertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->verticesSize * 3, mesh->verticesArray, GL_STATIC_DRAW);

		//Index buffer
		if (mesh->indexSize > 0)
		{
			glGenBuffers(1, &(mesh->idIndex));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->idIndex);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->indexSize, mesh->indicesArray, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}				

		//Textures buffer
		if (mesh->textSize > 0)
		{
			glGenBuffers(1, &mesh->idText);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->idText);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->textSize * 2, mesh->texturesArray, GL_STATIC_DRAW);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
		}

		//Normals buffer
		if (mesh->normalsSize > 0)
		{
			glGenBuffers(1, (GLuint*)&mesh->idNormals);
			glBindBuffer(GL_NORMAL_ARRAY, mesh->idNormals);
			glBufferData(GL_NORMAL_ARRAY, sizeof(float) * mesh->verticesSize * 3, mesh->normalsArray, GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(2);
		}
		glBindVertexArray(0);
	}
}

void Mesh::Render()
{	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	for (std::vector<RES_Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{		
		for (int i = 0; i < meshes.size(); i++)
		{
			glPushMatrix();
			glMultMatrixf((float*)&gameobject->transform->GetGlobalTransformTransposed());

			//glBindVertexArray(VAO);	
			glBindBuffer(GL_ARRAY_BUFFER, meshes[i]->idVertex);
			glVertexPointer(3, GL_FLOAT, 0, NULL);

			glBindBuffer(GL_NORMAL_ARRAY, meshes[i]->idNormals);
			glNormalPointer(GL_FLOAT, 0, NULL);

			//Normals
			if (meshes[i]->drawVertexNormals) DrawVertexNormals(meshes[i]);
			if (meshes[i]->drawFaceNormals) DrawFaceNormals(meshes[i]);

			if (meshes[i]->material != nullptr && drawTexture && meshes[i]->drawText)
			{
				glBindBuffer(GL_ARRAY_BUFFER, meshes[i]->idText);
				glTexCoordPointer(2, GL_FLOAT, 0, NULL);
				glBindTexture(GL_TEXTURE_2D, meshes[i]->material->textureID);
			}


			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i]->idIndex);
			glDrawElements(GL_TRIANGLES, meshes[i]->indexSize, GL_UNSIGNED_INT, NULL);

			glPopMatrix();
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_NORMAL_ARRAY, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		
	}
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Mesh::DrawVertexNormals(RES_Mesh* mesh)
{
	float length = 0.2f;
	glBegin(GL_LINES);
	for (size_t a = 0; a < mesh->verticesSize *3; a += 3)
	{
		glColor3f(0.0f, 0.85f, 0.85f);
		glVertex3f(mesh->verticesArray[a], mesh->verticesArray[a+1], mesh->verticesArray[a + 2]);

		glVertex3f(mesh->verticesArray[a] + (mesh->normalsArray[a] * length),
			mesh->verticesArray[a+1] + (mesh->normalsArray[a+1] * length),
			mesh->verticesArray[a+2] + (mesh->normalsArray[a+2]) * length);
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnd();

}

void Mesh::DrawFaceNormals(RES_Mesh* mesh)
{
	float length = 0.2f;
	glBegin(GL_LINES);
	for (size_t a = 0; a < mesh->verticesSize * 3; a += 3)
	{
		glColor3f(1.0f, 0.85f, 0.0f);
		float vx = (mesh->verticesArray[a] + mesh->verticesArray[a + 3] + mesh->verticesArray[a + 6]) / 3;
		float vy = (mesh->verticesArray[a + 1] + mesh->verticesArray[a + 4] + mesh->verticesArray[a + 7]) / 3;
		float vz = (mesh->verticesArray[a + 2] + mesh->verticesArray[a + 5] + mesh->verticesArray[a + 8]) / 3;

		float nx = (mesh->normalsArray[a] + mesh->normalsArray[a + 3] + mesh->normalsArray[a + 6]) / 3;
		float ny = (mesh->normalsArray[a + 1] + mesh->normalsArray[a + 4] + mesh->normalsArray[a + 7]) / 3;
		float nz = (mesh->normalsArray[a + 2] + mesh->normalsArray[a + 5] + mesh->normalsArray[a + 8]) / 3;

		glVertex3f(vx, vy, vz);

		glVertex3f(vx + (mesh->normalsArray[a] * length),
			vy + (mesh->normalsArray[a + 1] * length),
			vz + (mesh->normalsArray[a + 2]) * length);
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnd();
}

void Mesh::CreateCube()
{
	RES_Mesh* mesh = new RES_Mesh();
	mesh->name = "Cube mesh";
	GLuint index[] = { 0,1,2, 0,2,3, 1,4,5, 1,5,2, 6,0,3, 6,3,7, 3,2,5, 3,5,7, 6,4,1, 6,1,0, 4,6,7, 4,7,5 };
	GLfloat vertices[] = { 0.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,1.0f,0.0f, 0.0f,1.0f,0.0f,
		1.0f,0.0f,-1.0f, 1.0f,1.0f,-1.0f, 0.0f,0.0f,-1.0f, 0.0f,1.0f,-1.0f };

	mesh->verticesSize = (sizeof(vertices) / sizeof(*vertices)) / 3;
	mesh->verticesArray = new float[mesh->verticesSize * 3];
	memcpy(mesh->verticesArray, vertices, sizeof(float) * mesh->verticesSize * 3);

	mesh->indexSize = sizeof(index) / sizeof(*index);
	mesh->indicesArray = new uint[mesh->indexSize];
	memcpy(mesh->indicesArray, index, sizeof(uint) * mesh->indexSize);
	meshes.push_back(mesh);
	GenerateBuffers(meshes.back());
}

void Mesh::CreatePyramid()
{
	RES_Mesh* mesh = new RES_Mesh();
	mesh->name = "Piramid mesh";
	GLuint index[] = { 3,2,1, 3,1,0, 0,1,4, 1,2,4, 2,3,4, 3,0,4 };
	GLfloat vertices[] = { 0.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,0.0f,-1.0f, 0.0f,0.0f,-1.0f, 0.5f,1.0f,-0.5f };
	mesh->verticesSize = (sizeof(vertices) / sizeof(*vertices)) / 3;
	mesh->verticesArray = new float[mesh->verticesSize * 3];
	memcpy(mesh->verticesArray, vertices, sizeof(float) * mesh->verticesSize * 3);

	mesh->indexSize = sizeof(index) / sizeof(*index);
	mesh->indicesArray = new uint[mesh->indexSize];
	memcpy(mesh->indicesArray, index, sizeof(uint) * mesh->indexSize);
	meshes.push_back(mesh);
	GenerateBuffers(meshes.back());
}

void Mesh::CreatePlane(float size)
{
	RES_Mesh* mesh = new RES_Mesh();
	mesh->name = "Plane mesh";
	GLuint index[] = { 0,1,2,	0,2,3 };
	GLfloat vertices[] = { 0.0f,0.0f,0.0f,  size,0.0f,0.0f, size,0.0f,size,  0.0f,0.0f,size };
	mesh->verticesSize = (sizeof(vertices) / sizeof(*vertices)) / 3;
	mesh->verticesArray = new float[mesh->verticesSize * 3];
	memcpy(mesh->verticesArray, vertices, sizeof(float) * mesh->verticesSize * 3);

	mesh->indexSize = sizeof(index) / sizeof(*index);
	mesh->indicesArray = new uint[mesh->indexSize];
	memcpy(mesh->indicesArray, index, sizeof(uint) * mesh->indexSize);
	meshes.push_back(mesh);
	GenerateBuffers(meshes.back());
}

RES_Mesh* Mesh::CreateMesh(JSonHandler* file)
{
	RES_Mesh* mesh = new RES_Mesh();
	mesh->LoadMesh(file);
	meshes.push_back(mesh);
	return mesh;
}

void Mesh::SaveComponent(JSonHandler* file)
{
	JSonHandler node = file->InsertNodeArray("Components");
	node.SaveNum("CompType", (double)compType);
	//node.SaveString("UUID", UUID.c_str());
	node.SaveBool("Active",active);
	node.SaveBool("DrawTexture",drawTexture);
	node.SaveString("FBX_Path", pathFBX.c_str());
	node.CreateArray("Meshes");
	for (std::vector<RES_Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		(*it)->SaveMesh(&node,"Meshes");
	}
}

void Mesh::LoadComponent(JSonHandler* file)
{
	//JSonHandler node = file->GetNode("Mesh");
	UUID = file->GetString("UUID");
	active = file->GetBool("Active");
	drawTexture = file->GetBool("DrawTexture");
	pathFBX = file->GetString("FBX_Path");
	file->LoadArray("Meshes");
	int i = file->GetArrayCount("Meshes");
	if (i > 0)
	{
		for (int a = 0;a < i;a++)
		{
			JSonHandler tempnode = file->GetNodeArray("Meshes",a);
			RES_Mesh* temp = CreateMesh(&tempnode);
			LoadMesh(temp);
		}
	}
}