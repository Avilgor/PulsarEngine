#ifndef __RES_Mesh_H__
#define __RES_Mesh_H__

#include "Globals.h"
#include "EngineResource.h"
#include <string>

class RES_Material;
class JSonHandler;

class RES_Mesh : public EngineResource
{
public:
	RES_Mesh();
	RES_Mesh(std::string uuid);
	~RES_Mesh();
	void Clean();
	void SaveResource(JSonHandler* file);
	void LoadResource(JSonHandler* file);

public:

	uint VAO;
	uint idVertex;
	uint idIndex;
	uint idText;
	uint idNormals;
	uint verticesSize;
	uint indexSize;
	uint textSize;
	uint normalsSize;

	uint* indicesArray;
	float* verticesArray;
	float* normalsArray;
	float* texturesArray;
};

#endif //__RES_Mesh_H__