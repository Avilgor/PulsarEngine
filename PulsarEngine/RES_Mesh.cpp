#include "Application.h"
#include "RES_Mesh.h"
#include "RES_Material.h"
#include "JSonHandler.h"
#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"
#include "MathGeoLib/include/MathGeoLib.h"

RES_Mesh::RES_Mesh()
{
	name = "";
	VAO = 0;
	idVertex = 0;
	idIndex = 0;
	idText = 0;
	idNormals = 0;
	verticesSize = 0;
	indexSize = 0;
	textSize = 0;
	normalsSize = 0;
	drawText = true;
	UUID = App->GenerateUUID_V4();
	indicesArray = nullptr;
	verticesArray = nullptr;
	normalsArray = nullptr;
	texturesArray = nullptr;

	drawVertexNormals = false;
	drawFaceNormals = false;
	material = nullptr;
}

RES_Mesh::~RES_Mesh()
{

}

void RES_Mesh::Clean()
{
	glDeleteBuffers(1, &(GLuint)idIndex);
	glDeleteBuffers(1, &(GLuint)idNormals);
	glDeleteBuffers(1, &(GLuint)idVertex);
	glDeleteBuffers(1, &(GLuint)idText);

	delete indicesArray;
	delete verticesArray;
	delete normalsArray;
	delete texturesArray;

	indicesArray = nullptr;
	verticesArray = nullptr;
	normalsArray = nullptr;
	texturesArray = nullptr;
	name = "";
	VAO = 0;
	idVertex = 0;
	idIndex = 0;
	idText = 0;
	idNormals = 0;
	verticesSize = 0;
	indexSize = 0;
	textSize = 0;
	normalsSize = 0;
	drawText = true;
	material = nullptr;
	drawVertexNormals = false;
	drawFaceNormals = false;

}

void RES_Mesh::SaveMesh(JSonHandler* file, const char* label)
{
	JSonHandler node = file->InsertNodeArray(label);
	node.SaveString("UUID", UUID.c_str());
	node.SaveString("Name",name.c_str());
	node.SaveString("Path", path.c_str());
	node.SaveBool("DrawText",drawText);
	node.SaveString("MaterialUUID", materialUUID.c_str());

}

void RES_Mesh::LoadMesh(JSonHandler* file)
{
	name = file->GetString("Name");
	UUID = file->GetString("UUID");
	path = file->GetString("Path");
	drawText = file->GetBool("DrawText");
	materialUUID = file->GetString("MaterialUUID");
}


void RES_Mesh::SetMaterial(RES_Material* mat)
{
	material = mat;
	materialUUID = mat->UUID;
}