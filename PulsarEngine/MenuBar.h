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

private:
	
};

#endif //__MenuBar_H__