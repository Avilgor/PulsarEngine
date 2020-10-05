#ifndef __ConfigWindow_H__
#define __ConfigWindow_H__

#include "EditorWindow.h"
#include "Timer.h"
#include <string>

class ConfigWindow : public EditorWindow
{
public:
	ConfigWindow(std::string name);
	virtual ~ConfigWindow();

	void Draw();
	//void InfoProcessing();

private:
	bool fullscreen = WIN_FULLSCREEN;
	bool borderless = WIN_BORDERLESS;
	bool resizable = WIN_RESIZABLE;
	bool maximize = WIN_FULLSCREEN_DESKTOP;
	int width;
	int height;
	float brightness = 1.0f;
	float fps[100];
	float ms[100];
	Timer plotTimer;
};

#endif //__ConfigWindow_H__