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

class Material;
struct MaterialInfo;

struct MeshInfo 
{
	void Clean()
	{
		delete indicesArray;
		delete verticesArray;
		delete normalsArray ;
		delete texturesArray;

		material = nullptr;
	}
	void SetMaterial(MaterialInfo* mat)
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

	uint* indicesArray = nullptr;
	float* verticesArray = nullptr;
	float* normalsArray = nullptr;
	float* texturesArray = nullptr;

	bool drawVertexNormals = false;
	bool drawFaceNormals = false;

	MaterialInfo* material = nullptr;
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
	void SetAllMeshesMaterial(MaterialInfo* mat);
	MeshInfo* GetMesh(int index);
	std::vector<MeshInfo>* GetMeshes() { return &meshes; }
	void Render();
	void SetMaterial(Material* mat) { material = mat; }
	void GenerateBuffers();
	void CreateCube(float x, float y, float z);
	void CreatePyramid(float x, float y, float z);
	void CreateSphere(float radius, int rings, int sectors, float x, float y, float z);
	void CreatePlane(float size,float x, float y, float z);

public:

	std::string path;
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