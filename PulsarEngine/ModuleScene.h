#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include "Module.h"
#include "Timer.h"
#include "MathGeoLib/include/MathGeoLib.h"


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
	void RequestSave();
	void RequestLoad();
	void CreateNewScene();
	float GetTimeScale() { return timeScale; }
	void SetTimeScale(float val);
	float GetDeltaTime();
	float GetSceneRunningTime();
	void GetIntersectedGameobjects(std::vector<GameObject*> *vec,LineSegment ray);
	

private:
	void SaveScene();
	void LoadScene();

public:
	SceneState state;

private:
	float timeScale;
	bool save;
	bool load;
	float lastDT;
	Scene* activeScene;
	SceneState lastState;
	Timer sceneStartTimer; //No stop
	Timer sceneRunTime; //Stop when paused
};

#endif //__ModuleScene_H__