#ifndef __FBXLoaderModule_H__
#define __FBXLoaderModule_H__

#include "Globals.h"


class Mesh;

class FBXLoaderModule 
{
public:
	FBXLoaderModule();
	~FBXLoaderModule();

	bool ImportMesh(Mesh* mesh,const char* path);

};

#endif //__FBXLoaderModule_H__