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

}

void Material::UpdateComponent()
{

}

void Material::DeleteComponent()
{
	delete this;
}

void Material::SaveMaterial(MaterialInfo mat)
{
	materials.push_back(mat);
}


void Material::ChangeMaterial(MaterialInfo mat, int index)
{
	materials[index] = mat;
}

void Material::RemoveMaterial(int index)
{

}

void Material::DeleteMaterials()
{
	
}