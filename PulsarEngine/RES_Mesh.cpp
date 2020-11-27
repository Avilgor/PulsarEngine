#include "Application.h"
#include "RES_Mesh.h"
#include "JSonHandler.h"
#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"

RES_Mesh::RES_Mesh() : EngineResource(MESH_RES)
{
	VAO = 0;
	idVertex = 0;
	idIndex = 0;
	idText = 0;
	idNormals = 0;
	verticesSize = 0;
	indexSize = 0;
	textSize = 0;
	normalsSize = 0;
	indicesArray = nullptr;
	verticesArray = nullptr;
	normalsArray = nullptr;
	texturesArray = nullptr;
	meshRes = this;
	clean = false;
}

RES_Mesh::RES_Mesh(std::string id) : EngineResource(MESH_RES,id)
{
	VAO = 0;
	idVertex = 0;
	idIndex = 0;
	idText = 0;
	idNormals = 0;
	verticesSize = 0;
	indexSize = 0;
	textSize = 0;
	normalsSize = 0;
	indicesArray = nullptr;
	verticesArray = nullptr;
	normalsArray = nullptr;
	texturesArray = nullptr;

	clean = false;
}

RES_Mesh::~RES_Mesh()
{
	if (!clean)
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
		VAO = 0;
		idVertex = 0;
		idIndex = 0;
		idText = 0;
		idNormals = 0;
		verticesSize = 0;
		indexSize = 0;
		textSize = 0;
		normalsSize = 0;
	}
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
	VAO = 0;
	idVertex = 0;
	idIndex = 0;
	idText = 0;
	idNormals = 0;
	verticesSize = 0;
	indexSize = 0;
	textSize = 0;
	normalsSize = 0;
	clean = true;
}

void RES_Mesh::SaveResource(JSonHandler* file)
{
	file->SaveString("UUID", UUID.c_str());
	file->SaveString("Name",name.c_str());
	file->SaveNum("Type", (double)type);
	file->SaveString("LibPath", libPath.c_str());
	file->SaveString("AssetPath", assetPath.c_str());

	file->CreateArray("Position");
	file->InsertNumArray("Position",importPos.x);
	file->InsertNumArray("Position", importPos.y);
	file->InsertNumArray("Position", importPos.z);
	file->CreateArray("Rotation");
	file->InsertNumArray("Rotation", importRot.x);
	file->InsertNumArray("Rotation", importRot.y);
	file->InsertNumArray("Rotation", importRot.z);
	file->InsertNumArray("Rotation", importRot.w);
	file->CreateArray("Scale");
	file->InsertNumArray("Scale", importScale.x);
	file->InsertNumArray("Scale", importScale.y);
	file->InsertNumArray("Scale", importScale.z);
}

void RES_Mesh::LoadResource(JSonHandler* file)
{
	name = file->GetString("Name");
	UUID = file->GetString("UUID");
	libPath = file->GetString("LibPath");
	assetPath = file->GetString("AssetPath");
	
	file->LoadArray("Position");
	importPos.x = (float)file->GetNumArray("Position", 0);
	importPos.y = (float)file->GetNumArray("Position", 1);
	importPos.z = (float)file->GetNumArray("Position", 2);

	file->LoadArray("Rotation");
	importRot.x = (float)file->GetNumArray("Rotation", 0);
	importRot.y = (float)file->GetNumArray("Rotation", 1);
	importRot.z = (float)file->GetNumArray("Rotation", 2);
	importRot.w = (float)file->GetNumArray("Rotation", 3);

	file->LoadArray("Scale");
	importScale.x = (float)file->GetNumArray("Scale", 0);
	importScale.y = (float)file->GetNumArray("Scale", 1);
	importScale.z = (float)file->GetNumArray("Scale", 2);
}