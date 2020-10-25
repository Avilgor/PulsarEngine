#include "Globals.h"
#include "Material.h"
#include "Assimp/include/material.h"
#include "Assimp/include/texture.h"
#include "Devil/include/IL/ilu.h"
#include "Devil/include/IL/ilut.h"

#pragma comment( lib, "Devil/lib/x86/DevIL.lib" )
#pragma comment( lib, "Devil/lib/x86/ILU.lib" )
#pragma comment( lib, "Devil/lib/x86/ILUT.lib" )

Material::Material(GameObject* parent) : Component(parent, MATERIAL_COMP)
{
	component->material = this;
}

Material::~Material()
{
	if (!materials.empty())
	{
		for (std::vector<MaterialInfo>::iterator it = materials.begin(); it != materials.end(); ++it)
		{
			(*it).Clean();
		}
		materials.clear();
	}
}

void Material::UpdateComponent()
{

}

void Material::DeleteComponent()
{
	if (!materials.empty())
	{
		for (std::vector<MaterialInfo>::iterator it = materials.begin(); it != materials.end(); ++it)
		{
			(*it).Clean();
		}
		materials.clear();
	}
	delete this;
}

void Material::SaveMaterial(MaterialInfo mat)
{
	materials.push_back(mat);
}


void Material::ChangeMaterial(MaterialInfo mat, int index)
{
	if (materials.size() > index)
	{
		materials[index].Clean();
		materials[index] = mat;
	}
}

void Material::RemoveMaterial(int index)
{
	if (materials.size() > index)
	{
		materials[index].Clean();
		materials.erase(materials.begin() + index);
	}
}

void Material::DeleteMaterials()
{
	if (!materials.empty())
	{
		for (std::vector<MaterialInfo>::iterator it = materials.begin(); it != materials.end(); ++it)
		{
			(*it).Clean();
		}
		materials.clear();
	}
}