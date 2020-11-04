#ifndef __RES_Material_H__
#define __RES_Material_H__

#include "Globals.h"
#include "Color.h"

#include <string>

class JSonHandler;
typedef unsigned char GLubyte;

class RES_Material
{
public:

	RES_Material();
	~RES_Material();
	void Clean();
	void ChangeColor(float r, float g, float b, float a);
	void ChangeColor(Color col);
	void SaveMaterial(JSonHandler* file, const char* label);
	void LoadMaterial(JSonHandler* file);

public:
	GLubyte* textData = nullptr;
	uint texturesNum = 0;
	uint textureID = 0;
	std::string UUID;
	std::string texturePath = "";
	Color color = Color(1.0f, 1.0f, 1.0f);
	std::string name = "None";
	int bufferSize = 0;
	int textWidth = 0;
	int textHeight = 0;
};

#endif //__RES_Material_H__