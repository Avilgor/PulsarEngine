#ifndef __Mesh_H__
#define __Mesh_H__

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"

#include <string>

class Mesh : public Component
{
public:
	Mesh(GameObject* parent);
	~Mesh();

	void UpdateComponent();
	void DeleteComponent();
	bool LoadImportedMesh();
	void Render();
	void GenerateBuffers();
	void CreateCube(float x, float y, float z);
	void CreatePyramid(float x, float y, float z);
	void CreateSphere(float radius, int rings, int sectors, float x, float y, float z);
	void CreatePlane(float size,float x, float y, float z);

public:

	uint VAO;
	uint idVertex;
	uint idIndex;
	uint idText;
	uint idNormals;
	int verticesSize;
	uint indexSize;
	uint textSize;
	uint normalsSize;
	std::string path;

	uint* indicesArray = nullptr;
	float* verticesArray = nullptr;
	float* normalsArray = nullptr;
	float* texturesArray = nullptr;
};


#endif //__Mesh_H__