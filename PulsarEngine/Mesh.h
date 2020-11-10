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
class JSonHandler;

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
	void LoadMesh(RES_Mesh* mesh);
	void DrawVertexNormals(RES_Mesh* mesh);
	void DrawFaceNormals(RES_Mesh* mesh);
	void SetAllMeshesMaterial(RES_Material* mat);
	void SetMaterialByUUID(RES_Material* mat);
	RES_Mesh* GetMesh(int index);
	std::vector<RES_Mesh*> GetMeshes() { return meshes; }
	void Render();
	void SetMeshMaterial(RES_Material* mat,int index);
	void GenerateBuffers(RES_Mesh* mesh);
	void CreateCube();
	void CreatePyramid();
	void CreatePlane(float size);
	void SaveComponent(JSonHandler* file);
	void LoadComponent(JSonHandler* file);
	void UpdateAABB();
	AABB GetMeshAABB();

private:
	RES_Mesh* CreateMesh(JSonHandler* file);
public:

	std::string pathFBX = "";
	std::string name = "";
	bool drawTexture = true;
	bool drawAABB = false;
	
private:
	std::vector<RES_Mesh*> meshes;
	AABB aabb;
};


#endif //__Mesh_H__