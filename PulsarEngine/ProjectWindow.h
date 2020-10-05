#ifndef __ProjectWindow_H__
#define __ProjectWindow_H__

#include "EditorWindow.h"
#include "Globals.h"
#include <string>

class ProjectWindow : public EditorWindow
{
public:
	ProjectWindow(std::string name);
	virtual ~ProjectWindow();

	update_status Draw();

private:

};

#endif //__ProjectWindow_H__