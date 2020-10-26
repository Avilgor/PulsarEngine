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
	void CreateEmptyGameobject();


private:
	GameObject* root;
	GameObject* warrior;//temp
	GameObject* go2;//temp
};

#endif //__ModuleScene_H__