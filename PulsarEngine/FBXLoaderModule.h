#ifndef __FBXLoaderModule_H__
#define __FBXLoaderModule_H__

#include "Globals.h"


class Mesh;
class Material;

class FBXLoaderModule 
{
public:
	FBXLoaderModule();
	~FBXLoaderModule();

	bool ImportMesh(Mesh* mesh,const char* path);
	bool ImportMaterial(Material* mesh, const char* path);
	bool ImportAll(Mesh* mesh, Material* mat, const char* path);
	void Save();
	void Load();


};

#endif //__FBXLoaderModule_H__