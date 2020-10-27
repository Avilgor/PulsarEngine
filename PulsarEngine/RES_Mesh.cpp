#include "RES_Mesh.h"

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


void RES_Mesh::SetMaterial(RES_Material* mat)
{
	material = mat;
}