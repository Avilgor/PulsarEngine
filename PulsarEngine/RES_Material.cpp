#include "Globals.h"
#include "Application.h"
#include "Color.h"
#include "RES_Material.h"
#include "JSonHandler.h"
#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"

RES_Material::RES_Material() : EngineResource(MATERIAL_RES)
{
	textData = nullptr;
	texturesNum = 0;
	textureID = 0;
	color = Color(1.0f, 1.0f, 1.0f);
	textWidth = 0;
	textHeight = 0;
	clean = false;
	matRes = this;
}

RES_Material::RES_Material(std::string id) : EngineResource(MATERIAL_RES,id)
{
	textData = nullptr;
	texturesNum = 0;
	textureID = 0;
	color = Color(1.0f, 1.0f, 1.0f);
	textWidth = 0;
	textHeight = 0;
	clean = false;
	matRes = this;
}


RES_Material::~RES_Material()
{
	if (!clean)
	{
		App->fileSystem->UnloadTexure(textureID);
		textData = nullptr;
		texturesNum = 0;
		textureID = 0;
		color = Color(1.0f, 1.0f, 1.0f);
		textWidth = 0;
		textHeight = 0;
	}
}

void RES_Material::Clean()
{
	App->fileSystem->UnloadTexure(textureID);
	textData = nullptr;
	texturesNum = 0;
	textureID = 0;
	color = Color(1.0f, 1.0f, 1.0f);
	textWidth = 0;
	textHeight = 0;
	clean = true;
}

void RES_Material::ChangeColor(float r, float g, float b, float a)
{
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}
void RES_Material::ChangeColor(Color col)
{
	color = col;
}

void RES_Material::SaveResource(JSonHandler* file)
{
	file->SaveString("UUID", UUID.c_str());
	file->SaveString("Name", name.c_str());
	file->SaveNum("Type", (double)type);
	file->SaveString("LibPath", libPath.c_str());
	file->SaveString("AssetPath", assetPath.c_str());
	file->SaveNum("ColorR",color.r);
	file->SaveNum("ColorG", color.g);
	file->SaveNum("ColorB", color.b);
	file->SaveNum("ColorA", color.a);
}

void RES_Material::LoadResource(JSonHandler* file)
{
	name = file->GetString("Name");
	UUID = file->GetString("UUID");
	libPath = file->GetString("LibPath");
	assetPath = file->GetString("AssetPath");
	color.r = file->GetNum("ColorR");
	color.g = file->GetNum("ColorG");
	color.b = file->GetNum("ColorB");
	color.a = file->GetNum("ColorA");
}