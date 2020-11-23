#ifndef __Mesh_H__
#define __Mesh_H__

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Math/float4x4.h"

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
	
	void SetMesh(RES_Mesh* mesh);
	void RemoveMesh();
	void UpdateComponent();
	void DeleteComponent();
	void DrawVertexNormals();
	void DrawFaceNormals();
	RES_Mesh* GetMesh() { return resMesh; }
	RES_Material* GetMaterial() { return resMat; }
	void Render();
	void SetMaterial(RES_Material* mat);

	void SaveComponent(JSonHandler* file);
	void LoadComponent(JSonHandler* file);

	void UpdateAABB();
	AABB GetMeshAABB();

public:

	bool drawTexture = true;
	bool drawAABB = false;
	bool drawFaceNormals = false;
	bool drawVertexNormals = false;

private:
	RES_Mesh* resMesh = nullptr;
	RES_Material* resMat = nullptr;
	AABB aabb;
};


#endif //__Mesh_H__