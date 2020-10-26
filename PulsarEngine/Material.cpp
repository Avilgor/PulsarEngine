#include "Globals.h"
#include "Application.h"
#include "FileSystemModule.h"
#include "Material.h"
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

void Material::GenerateBuffer()
{
	if (!materials.empty())
	{
		for (std::vector<MaterialInfo>::iterator it = materials.begin(); it != materials.end(); ++it)
		{
			std::string trick = "Assets/3D/Baker/";
			trick.append((*it).path.c_str());
			App->fileSystem->LoadTexture(trick.c_str(),&(*it));
			if ((*it).textureID != -1)
			{
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glGenTextures(1, &(*it).textureID);
				glBindTexture(GL_TEXTURE_2D, (*it).textureID);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (*it).textWidth, (*it).textHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (*it).textData);
				glGenerateMipmap(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	}
	
}

MaterialInfo* Material::GetMaterial(int index)
{
	MaterialInfo* ret = nullptr;
	if (!materials.empty() && index < materials.size())
	{
		ret = &materials[index];
	}
	return ret;
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