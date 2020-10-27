#ifndef __Mesh_H__
#define __Mesh_H__

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"

#include <string>
#include <vector>

class RES_Material;

struct MeshInfo 
{
	void Clean()
	{
		delete indicesArray;
		delete verticesArray;
		delete normalsArray ;
		delete texturesArray;

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

	void SetMaterial(RES_Material* mat)
	{
		material = mat;
	}
	std::string name = "";
	uint VAO = 0;
	uint idVertex = 0;
	uint idIndex = 0;
	uint idText = 0;
	uint idNormals = 0;
	int verticesSize = 0;
	uint indexSize = 0;
	uint textSize = 0;
	uint normalsSize = 0;
	bool drawText = true;
	uint* indicesArray = nullptr;
	float* verticesArray = nullptr;
	float* normalsArray = nullptr;
	float* texturesArray = nullptr;

	bool drawVertexNormals = false;
	bool drawFaceNormals = false;
	RES_Material* material = nullptr;
};

class Mesh : public Component
{
public:
	Mesh(GameObject* parent);
	~Mesh();
	
	void AddMesh(MeshInfo mesh);
	void RemoveMesh(int index);
	void ReplaceMesh(MeshInfo mesh, int index);
	void UpdateComponent();
	void DeleteComponent();
	bool LoadImportedMesh();
	void DrawVertexNormals(MeshInfo mesh);
	void DrawFaceNormals(MeshInfo mesh);
	void SetAllMeshesMaterial(RES_Material* mat);
	MeshInfo* GetMesh(int index);
	std::vector<MeshInfo>* GetMeshes() { return &meshes; }
	void Render();
	void SetMeshMaterial(RES_Material* mat,int index);
	void GenerateBuffers(MeshInfo* mesh);
	void CreateCube();
	void CreatePyramid();
	void CreatePlane(float size);

public:

	std::string path;
	bool drawTexture = true;
	/*std::string name;
	uint VAO;
	uint idVertex;
	uint idIndex;
	uint idText;
	uint idNormals;
	int verticesSize;
	uint indexSize;
	uint textSize;
	uint normalsSize;
	
	uint* indicesArray = nullptr;
	float* verticesArray = nullptr;
	float* normalsArray = nullptr;
	float* texturesArray = nullptr;
	
	Material* material = nullptr;*/
	
private:
	std::vector<MeshInfo> meshes;
	
};


#endif //__Mesh_H__