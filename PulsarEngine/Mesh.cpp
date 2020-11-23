#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "RES_Mesh.h"
#include "Material.h"
#include "JSonHandler.h"
#include "ResourceManager.h"
#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include "MathGeoLib/include/Geometry/AABB.h"

//#include <vector>

Mesh::Mesh(GameObject* parent) : Component(parent, MESH_COMP)
{
	component->mesh = this;
	drawTexture = true;
	aabb.SetNegativeInfinity();
}

Mesh::~Mesh()
{}

void Mesh::UpdateComponent()
{
	if (resMesh != nullptr)
	{
		if (drawAABB) App->renderer3D->RenderAABB(aabb);		
		if(active) Render();
	}
}

void Mesh::DeleteComponent()
{
	if (resMesh != nullptr)
	{	
		App->resourceManager->FreeResource(resMesh->UUID);
		resMesh = nullptr;
	}
	if (resMat != nullptr)
	{
		App->resourceManager->FreeResource(resMat->UUID);
		resMat = nullptr;
	}
	
	delete this;
}

void Mesh::UpdateAABB()
{
	aabb.Enclose((float3*)resMesh->verticesArray, resMesh->verticesSize);
	gameobject->UpdateAABB();
}

AABB Mesh::GetMeshAABB()
{
	return aabb;
}

void Mesh::SetMesh(RES_Mesh* m)
{
	if (resMesh != nullptr) App->resourceManager->FreeResource(resMesh->UUID);
	resMesh = m;
	m->references++;
	UpdateAABB();
}

void Mesh::RemoveMesh()
{
	if (resMesh != nullptr) App->resourceManager->FreeResource(resMesh->UUID);
	resMesh = nullptr;
}

void Mesh::SetMaterial(RES_Material* m)
{
	if (resMat != nullptr) App->resourceManager->FreeResource(resMat->UUID);
	resMat = m;
	m->references++;
}


void Mesh::Render()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glPushMatrix();
	glMultMatrixf((float*)&gameobject->transform->GetGlobalTransformTransposed());

	//glBindVertexArray(VAO);	
	glBindBuffer(GL_ARRAY_BUFFER, resMesh->idVertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_NORMAL_ARRAY, resMesh->idNormals);
	glNormalPointer(GL_FLOAT, 0, NULL);

	//Normals
	if (drawVertexNormals) DrawVertexNormals();
	if (drawFaceNormals) DrawFaceNormals();

	if (resMat != nullptr && drawTexture)
	{
		glBindBuffer(GL_ARRAY_BUFFER, resMesh->idText);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		glBindTexture(GL_TEXTURE_2D, resMat->textureID);
	}


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resMesh->idIndex);
	glDrawElements(GL_TRIANGLES, resMesh->indexSize, GL_UNSIGNED_INT, NULL);

	glPopMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_NORMAL_ARRAY, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
}

void Mesh::DrawVertexNormals()
{
	float length = 0.2f;
	glBegin(GL_LINES);
	for (size_t a = 0; a < resMesh->verticesSize *3; a += 3)
	{
		glColor3f(0.0f, 0.85f, 0.85f);
		glVertex3f(resMesh->verticesArray[a], resMesh->verticesArray[a+1], resMesh->verticesArray[a + 2]);

		glVertex3f(resMesh->verticesArray[a] + (resMesh->normalsArray[a] * length),
			resMesh->verticesArray[a+1] + (resMesh->normalsArray[a+1] * length),
			resMesh->verticesArray[a+2] + (resMesh->normalsArray[a+2]) * length);
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnd();

}

void Mesh::DrawFaceNormals()
{
	float length = 0.2f;
	glBegin(GL_LINES);
	for (size_t a = 0; a < resMesh->verticesSize * 3; a += 3)
	{
		glColor3f(1.0f, 0.85f, 0.0f);
		float vx = (resMesh->verticesArray[a] + resMesh->verticesArray[a + 3] + resMesh->verticesArray[a + 6]) / 3;
		float vy = (resMesh->verticesArray[a + 1] + resMesh->verticesArray[a + 4] + resMesh->verticesArray[a + 7]) / 3;
		float vz = (resMesh->verticesArray[a + 2] + resMesh->verticesArray[a + 5] + resMesh->verticesArray[a + 8]) / 3;
					
		float nx = (resMesh->normalsArray[a] + resMesh->normalsArray[a + 3] + resMesh->normalsArray[a + 6]) / 3;
		float ny = (resMesh->normalsArray[a + 1] + resMesh->normalsArray[a + 4] + resMesh->normalsArray[a + 7]) / 3;
		float nz = (resMesh->normalsArray[a + 2] + resMesh->normalsArray[a + 5] + resMesh->normalsArray[a + 8]) / 3;

		glVertex3f(vx, vy, vz);

		glVertex3f(vx + (resMesh->normalsArray[a] * length),
			vy + (resMesh->normalsArray[a + 1] * length),
			vz + (resMesh->normalsArray[a + 2]) * length);
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnd();
}

void Mesh::SaveComponent(JSonHandler* file)
{
	JSonHandler node = file->InsertNodeArray("Components");
	node.SaveNum("CompType", (double)compType);
	node.SaveString("CompUUID", UUID.c_str());
	node.SaveBool("Active",active);
	node.SaveBool("DrawTexture",drawTexture);
	node.SaveString("MeshUUID", resMesh->UUID.c_str());
}

void Mesh::LoadComponent(JSonHandler* file)
{
	UUID = file->GetString("CompUUID");
	active = file->GetBool("Active");
	drawTexture = file->GetBool("DrawTexture");
	EngineResource* res = App->resourceManager->GetResource(file->GetString("MeshUUID"));
	if (res != nullptr)
	{
		resMesh = res->AsMesh();
		resMesh->references++;
	}
	UpdateAABB();
}