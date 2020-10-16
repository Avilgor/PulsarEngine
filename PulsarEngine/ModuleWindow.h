#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

#include "MathGeoLib\include\MathGeoLib.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);
	void SetWindowsSize(int width,int height);
	void GetWindowsSize(int &width,int &height);
	void SetFullscreen(bool val);
	void SetBorderless(bool val);
	void SetMaximize(bool val);
	void SetResizable(bool val);
	void SetBrightness(float val);
	void RestoreDefaultSize();
public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	int width;
	int height;
};

#endif // __ModuleWindow_H__