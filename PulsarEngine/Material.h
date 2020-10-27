#ifndef __Material_H__
#define __Material_H__

#include "Globals.h"
#include "Color.h"
#include "Component.h"
#include "RES_Material.h"

#include <vector>


class Material : public Component
{
public:

	Material(GameObject* parent);
	~Material();

	void UpdateComponent();
	void DeleteComponent();
	void GenerateBuffers();
	void GenerateBuffer(RES_Material* mat);
	void SaveMaterial(RES_Material* mat);
	void LoadTextureMaterial(std::string path);
	RES_Material* GetMaterial(int index);
	RES_Material* GetLastMaterial();
	std::vector<RES_Material*> GetAllMaterials() { return materials; }
	void ChangeMaterial(RES_Material* mat, int index);
	void RemoveMaterial(int index);
	void DeleteMaterials();

public:
	
	std::vector<RES_Material*> materials;
};

#endif //__Material_H__