#ifndef __FBXLoaderModule_H__
#define __FBXLoaderModule_H__

#include "Globals.h"


class Mesh;
class Material;
struct aiScene;
class RES_Mesh;

class FBXLoaderModule 
{
public:
	FBXLoaderModule();
	~FBXLoaderModule();

	bool ImportMesh(Mesh* mesh,const char* path);
	bool ImportMaterialFBX(Material* mesh, const char* path);
	bool ImportMaterialFBX(Material* mesh, const char* pathfbx, const char* pathtext);
	bool ImportAll(Mesh* mesh, Material* mat, const char* path);
	void Save();
	void Load();
	RES_Mesh* LoadMesh(int num, const aiScene* scene);

};

#endif //__FBXLoaderModule_H__