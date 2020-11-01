#include "Globals.h"
#include "Color.h"
#include "RES_Material.h"
#include "Glew/include/GL/glew.h"
#include "SDL/include/SDL_opengl.h"

RES_Material::RES_Material()
{
	textData = nullptr;
	texturesNum = 0;
	textureID = 0;
	path = "";
	color = Color(1.0f, 1.0f, 1.0f);
	name = "";
	textWidth = 0;
	textHeight = 0;
}

RES_Material::~RES_Material()
{}

void RES_Material::Clean()
{
	//glDeleteBuffers(1, &(GLuint)textureID);
	//delete textData;
	textData = nullptr;
	texturesNum = 0;
	textureID = 0;
	path = "";
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