#include "Globals.h"
#include "Application.h"
#include "FileSystemModule.h"
#include "Material.h"
#include "RES_Material.h"
#include "Mesh.h"
#include "JSonHandler.h"



Material::Material(GameObject* parent) : Component(parent, MATERIAL_COMP)
{
	component->material = this;
}

Material::~Material()
{
	if (material != nullptr) App->resourceManager->FreeResource(material->UUID);
	material = nullptr;
}

void Material::UpdateComponent()
{
}

void Material::DeleteComponent()
{
	if (material != nullptr) App->resourceManager->FreeResource(material->UUID);
	material = nullptr;
	delete this;
}


void Material::SetMaterial(RES_Material* mat)
{
	if (material != nullptr) App->resourceManager->FreeResource(material->UUID);
	resMaterial = mat;
	mat->references++;
}

void Material::DeleteMaterial()
{
	if(material != nullptr) App->resourceManager->FreeResource(material->UUID);
	material = nullptr;
}

void Material::SaveComponent(JSonHandler* file)
{
	JSonHandler node = file->InsertNodeArray("Components");
	node.SaveNum("CompType", (double)compType);
	node.SaveString("UUID", UUID.c_str());
	node.SaveBool("Active", active);
	node.SaveString("ResUUID", resMaterial->UUID.c_str());
}

void Material::LoadComponent(JSonHandler* file)
{
	Mesh* meshComp = nullptr; 
	if (gameobject->GetFirstComponentType(MESH_COMP) != nullptr) meshComp = gameobject->GetFirstComponentType(MESH_COMP)->AsMesh();
	UUID = file->GetString("UUID");
	active = file->GetBool("Active");	
	EngineResource* res = App->resourceManager->GetResource(file->GetString("ResUUID"));
	if (res != nullptr) 
	{
		resMaterial = res->AsMaterial();
		resMaterial->references++;
	}
	if (resMaterial != nullptr) meshComp->SetMaterial(resMaterial);
}