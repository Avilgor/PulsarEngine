#include "Globals.h"
#include "Material.h"

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

}