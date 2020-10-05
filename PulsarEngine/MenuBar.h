#ifndef __MenuBar_H__
#define __MenuBar_H__

#include "EditorWindow.h"
#include "Globals.h"
#include <string>

class MenuBar : public EditorWindow
{
public:
	MenuBar(std::string name);
	virtual ~MenuBar();

	update_status Draw();
	void DrawRenderOptions();
	void ToggleRenderOptions();

private:
	bool renderOptions = false;
	bool depth = false;
	bool cullFace = true;
	bool light = true;
	bool colorMaterial = true;
	bool texture2D = true;
	bool blend = true;
};

#endif //__MenuBar_H__