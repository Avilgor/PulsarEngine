#include "Globals.h"
#include "Application.h"
#include "FileSystemModule.h"
#include "Material.h"
#include "RES_Material.h"
#include "Assimp/include/material.h"
#include "Assimp/include/texture.h"
#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"


Material::Material(GameObject* parent) : Component(parent, MATERIAL_COMP)
{
	component->material = this;
}

Material::~Material()
{
	if (!materials.empty())
	{
		for (std::vector<RES_Material*>::iterator it = materials.begin(); it != materials.end(); ++it)
		{
			(*it)->Clean();
			delete (*it);
		}
		materials.clear();
	}
}

void Material::UpdateComponent()
{
}

void Material::GenerateBuffers()
{
	if (!materials.empty())
	{
		for (std::vector<RES_Material*>::iterator it = materials.begin(); it != materials.end(); ++it)
		{
			App->fileSystem->LoadTexture((*it)->path.c_str(), (*it));
			if ((*it)->textureID != -1)
			{
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glGenTextures(1, &(*it)->textureID);
				glBindTexture(GL_TEXTURE_2D, (*it)->textureID);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (*it)->textWidth, (*it)->textHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (*it)->textData);
				glGenerateMipmap(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	}

}

void Material::GenerateBuffer(RES_Material* mat)
{
	if (mat != nullptr)
	{
		App->fileSystem->LoadTexture(mat->path.c_str(), mat);
		if (mat->textureID != -1)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &mat->textureID);
			glBindTexture(GL_TEXTURE_2D, mat->textureID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mat->textWidth, mat->textHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mat->textData);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

void Material::LoadTextureNewMaterial(std::string pathtext)
{
	RES_Material* mat = new RES_Material();
	mat->name = "Material texture";
	mat->path = pathtext;
	mat->texturesNum = 1;	
	GenerateBuffer(mat);
	materials.push_back(mat);
}

RES_Material* Material::GetMaterial(int index)
{
	RES_Material* ret = nullptr;
	if (!materials.empty() && index < materials.size())
	{
		ret = materials[index];
	}
	return ret;
}

RES_Material* Material::GetLastMaterial()
{
	RES_Material* ret = nullptr;
	if (!materials.empty())
	{
		ret = materials[materials.size() - 1];
	}
	return ret;
}

void Material::DeleteComponent()
{
	if (!materials.empty())
	{
		for (std::vector<RES_Material*>::iterator it = materials.begin(); it != materials.end(); ++it)
		{
			(*it)->Clean();
			delete(*it);
		}
		materials.clear();
	}
	delete this;
}

void Material::SaveMaterial(RES_Material* mat)
{
	materials.push_back(mat);
}


void Material::ChangeMaterial(RES_Material* mat, int index)
{
	if (materials.size() > index)
	{
		materials[index]->Clean();
		delete materials[index];
		materials[index] = mat;
	}
}

void Material::RemoveMaterial(int index)
{
	if (materials.size() > index)
	{
		materials[index]->Clean();
		delete materials[index];
		materials.erase(materials.begin() + index);
	}
}

void Material::DeleteMaterials()
{
	if (!materials.empty())
	{
		for (std::vector<RES_Material*>::iterator it = materials.begin(); it != materials.end(); ++it)
		{
			(*it)->Clean();
			delete (*it);
		}
		materials.clear();
	}
}

void Material::ChangeMaterialTexture(const char* path, int index)
{
	if (!materials.empty() && index < materials.size())
	{
		materials[0]->Clean();
		materials[0]->name = "Material texture";
		materials[0]->path = path;
		materials[0]->texturesNum = 1;
		LoadMaterial(materials[0]);
	}
}

void Material::ChangeAllMaterialsTextures(const char* path)
{
	if (!materials.empty())
	{
		for (std::vector<RES_Material*>::iterator it = materials.begin(); it != materials.end(); ++it)
		{
			(*it)->Clean();
			(*it)->name = "Material texture";
			(*it)->path = path;
			(*it)->texturesNum = 1;
			LoadMaterial((*it));
		}		
	}
}

void Material::LoadMaterial(RES_Material* mat)
{	
	GenerateBuffer(mat);
}