#ifndef __EditorWindow_H__
#define __EditorWindow_H__

#include "SDL/include/SDL.h"
#include <string>

class Application;

class EditorWindow
{
public:
	EditorWindow(std::string name);
	virtual ~EditorWindow();

	virtual void Draw() {}
	virtual void InfoProcessing() {}
	void SetActive(bool active);
	bool IsActive() const { return active; }


public:
	std::string name;

protected:
	bool active = true;
};

#endif //__EditorWindow_H__