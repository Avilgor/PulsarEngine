#ifndef __FBXLoaderModule_H__
#define __FBXLoaderModule_H__

#include "Module.h"
#include "Globals.h"
#include "Mesh.h"

#include <list>


class FBXLoaderModule : public Module
{
public:
	FBXLoaderModule(Application* app, bool start_enabled = true);
	~FBXLoaderModule();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	bool ImportMesh(const char* path);

private:
	std::list<Mesh*> Meshes;
};

#endif //__FBXLoaderModule_H__