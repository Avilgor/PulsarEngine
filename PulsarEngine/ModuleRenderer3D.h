#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	//RenderOptions
	void ToggleShaded();
	void ToggleWireframe();
	void ToggleLights(bool val);
	void ToggleCullFace(bool val);
	void ToggleDepthTest(bool val);
	void ToggleColorMaterial(bool val);
	void Toggle2DTextures(bool val);
	void ToggleBlend(bool val);
	void ResetToDefault();

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};
