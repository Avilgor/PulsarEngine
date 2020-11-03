#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include "Module.h"

class Scene;

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
	Scene* GetActiveScene();
	void SaveCurrentScene();
	void LoadNewScene();


private:
	Scene* activeScene;
};

#endif //__ModuleScene_H__