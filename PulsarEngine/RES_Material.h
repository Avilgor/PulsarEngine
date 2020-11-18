#ifndef __RES_Material_H__
#define __RES_Material_H__

#include "Globals.h"
#include "EngineResource.h"
#include "Color.h"

#include <string>

class JSonHandler;
typedef unsigned char GLubyte;

class RES_Material : public EngineResource
{
public:

	RES_Material();
	RES_Material(std::string uuid);
	~RES_Material();
	void Clean();
	void ChangeColor(float r, float g, float b, float a);
	void ChangeColor(Color col);
	void SaveResource(JSonHandler* file);
	void LoadResource(JSonHandler* file);

public:
	GLubyte* textData = nullptr;
	uint texturesNum = 0;
	uint textureID = 0;

	Color color = Color(1.0f, 1.0f, 1.0f);
	int bufferSize = 0;
	int textWidth = 0;
	int textHeight = 0;
};

#endif //__RES_Material_H__