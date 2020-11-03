#ifndef __Material_H__
#define __Material_H__

#include "Globals.h"
#include "Color.h"
#include "Component.h"
#include "RES_Material.h"

#include <vector>

class JSonHandler;

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
	void LoadTextureNewMaterial(std::string path);
	void LoadMaterial(RES_Material* mat);
	RES_Material* GetMaterial(int index);
	RES_Material* GetLastMaterial();
	std::vector<RES_Material*> GetAllMaterials() { return materials; }
	void ChangeMaterial(RES_Material* mat, int index);
	void RemoveMaterial(int index);
	void DeleteMaterials();
	void ChangeMaterialTexture(const char* path,int index);
	void ChangeAllMaterialsTextures(const char* path);
	void SaveComponent(JSonHandler* file);
	void LoadComponent(JSonHandler* file, const char* label);

public:
	
	std::vector<RES_Material*> materials;
};

#endif //__Material_H__