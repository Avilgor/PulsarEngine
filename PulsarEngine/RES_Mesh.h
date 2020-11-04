#ifndef __RES_Mesh_H__
#define __RES_Mesh_H__

#include "Globals.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include <string>

class RES_Material;
class JSonHandler;

class RES_Mesh
{
public:
	RES_Mesh();
	~RES_Mesh();
	void Clean();
	void SetMaterial(RES_Material* mat);
	void UpdateAABB();
	void SaveMesh(JSonHandler* file, const char* label);
	void LoadMesh(JSonHandler* file);

public:
	std::string name="";
	std::string path="";
	uint VAO;
	uint idVertex;
	uint idIndex;
	uint idText;
	uint idNormals;
	uint verticesSize;
	uint indexSize;
	uint textSize;
	uint normalsSize;
	bool drawText;
	uint* indicesArray;
	float* verticesArray;
	float* normalsArray;
	float* texturesArray;
	AABB aabb;
	std::string UUID="";
	std::string materialUUID = "";
	bool drawVertexNormals;
	bool drawFaceNormals;
	RES_Material* material;
};

#endif //__RES_Mesh_H__