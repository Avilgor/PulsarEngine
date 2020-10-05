#ifndef __MenuBar_H__
#define __MenuBar_H__

#include "EditorWindow.h"
#include <string>

class MenuBar : public EditorWindow
{
public:
	MenuBar(std::string name);
	virtual ~MenuBar();

	void Draw();
	void DrawRenderOptions();
	void ToggleRenderOptions();

private:
	bool renderOptions = false;
	bool depth = false;
	bool cullFace = true;
	bool light = true;
	bool colorMaterial = true;
	bool texture2D = true;
};

#endif //__MenuBar_H__