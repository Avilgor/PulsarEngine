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
class RES_Mesh;

class Mesh : public Component
{
public:
	Mesh(GameObject* parent);
	~Mesh();
	
	void AddMesh(RES_Mesh* mesh);
	void RemoveMesh(int index);
	void ReplaceMesh(RES_Mesh* mesh, int index);
	void UpdateComponent();
	void DeleteComponent();
	bool LoadImportedMesh();
	void DrawVertexNormals(RES_Mesh* mesh);
	void DrawFaceNormals(RES_Mesh* mesh);
	void SetAllMeshesMaterial(RES_Material* mat);
	RES_Mesh* GetMesh(int index);
	std::vector<RES_Mesh*> GetMeshes() { return meshes; }
	void Render();
	void SetMeshMaterial(RES_Material* mat,int index);
	void GenerateBuffers(RES_Mesh* mesh);
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
	std::vector<RES_Mesh*> meshes;
	
};


#endif //__Mesh_H__