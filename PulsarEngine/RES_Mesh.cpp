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
}

void RES_Mesh::LoadResource(JSonHandler* file)
{
	name = file->GetString("Name");
	UUID = file->GetString("UUID");
	libPath = file->GetString("LibPath");
	assetPath = file->GetString("AssetPath");
}