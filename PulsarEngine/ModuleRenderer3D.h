#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "Glew\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "Glew/lib/Win32/glew32.lib") 

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
	void UpdateProjection();
	void OnResize(int width, int height,int offsetX = 0,int offsetY = 0);
	void RenderGroundGrid(int size);
	void RenderAABB(AABB box);
	void RenderBox(float3 corners[8]);
	void RenderLine(float3 A,float3 B);

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

	uint frameBuffer = 0;
	uint renderBuffer = 0;
	uint renderTexture = 0;
	uint depthTexture = 0;
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	//mat3x3 NormalMatrix;
	//mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};
