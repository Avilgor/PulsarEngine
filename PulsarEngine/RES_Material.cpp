#include "Globals.h"
#include "Application.h"
#include "Color.h"
#include "RES_Material.h"
#include "JSonHandler.h"
#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"

RES_Material::RES_Material()
{
	textData = nullptr;
	texturesNum = 0;
	textureID = 0;
	texturePath = "";
	color = Color(1.0f, 1.0f, 1.0f);
	name = "";
	textWidth = 0;
	textHeight = 0;
	UUID = App->GenerateUUID_V4();
}

RES_Material::~RES_Material()
{}

void RES_Material::Clean()
{
	//glDeleteBuffers(1, &(GLuint)textureID);
	//delete textData;
	App->fileSystem->UnloadTexure(textureID);
	textData = nullptr;
	texturesNum = 0;
	textureID = 0;
	texturePath = "";
	color = Color(1.0f, 1.0f, 1.0f);
	name = "";
	textWidth = 0;
	textHeight = 0;
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

void RES_Material::SaveMaterial(JSonHandler* file, const char* label)
{
	JSonHandler node = file->InsertNodeArray(label);
	node.SaveString("UUID", UUID.c_str());
	node.SaveString("Name", name.c_str());
	node.SaveString("Path", texturePath.c_str());

	node.SaveNum("ColorR",color.r);
	node.SaveNum("ColorG", color.g);
	node.SaveNum("ColorB", color.b);
	node.SaveNum("ColorA", color.a);
}

void RES_Material::LoadMaterial(JSonHandler* file)
{
	name = file->GetString("Name");
	UUID = file->GetString("UUID");
	texturePath = file->GetString("Path");
	color.r = file->GetNum("ColorR");
	color.g = file->GetNum("ColorG");
	color.b = file->GetNum("ColorB");
	color.a = file->GetNum("ColorA");
}