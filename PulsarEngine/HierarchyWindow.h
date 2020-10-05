#ifndef __HierarchyWindow_H__
#define __HierarchyWindow_H__

#include "EditorWindow.h"
#include "Globals.h"
#include <string>

class HierarchyWindow : public EditorWindow
{
public:
	HierarchyWindow(std::string name);
	virtual ~HierarchyWindow();

	update_status Draw();


private:
};

#endif //__HierarchyWindow_H__