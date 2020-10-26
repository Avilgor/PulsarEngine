#ifndef __Material_H__
#define __Material_H__

#include "Globals.h"
#include "Color.h"
#include "Component.h"

#include <vector>
#include <string>

struct aiMaterial;

struct MaterialInfo
{
	void Clean()
	{
		delete mat;
		mat = nullptr;
	}
	void ChangeColor(float r, float g, float b, float a)
	{
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}
	void ChangeColor(Color col)
	{
		color = col;
	}
public:
	aiMaterial* mat;
	uint texturesNum;
	uint textureID;
	std::string path;
	Color color;
	std::string name;
};

class Material : public Component
{
public:

	Material(GameObject* parent);
	~Material();

	void UpdateComponent();
	void DeleteComponent();
	void SaveMaterial(MaterialInfo mat);
	std::vector<MaterialInfo>* GetAllMaterials() { return &materials; }
	void ChangeMaterial(MaterialInfo mat, int index);
	void RemoveMaterial(int index);
	void DeleteMaterials();

public:
	
	std::vector<MaterialInfo> materials;
};

#endif //__Material_H__