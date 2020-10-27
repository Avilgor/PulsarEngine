#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include "Module.h"
#include "GameObject.h"


class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	GameObject* GetRoot() { return root; }
	GameObject* CreateEmptyGameobject();
	GameObject* CreateEmptyGameobject(const char* name);
	void CreateCube();
	void CreatePyramid();
	void CreatePlane();


private:
	GameObject* root;
};

#endif //__ModuleScene_H__