#ifndef __ConsoleWindow_H__
#define __ConsoleWindow_H__

#include "EditorWindow.h"
#include <string>

class ConsoleWindow : public EditorWindow
{
public:
	ConsoleWindow(std::string name);
	virtual ~ConsoleWindow();

	void Draw();

private:
	
};

#endif //__ConsoleWindow_H__