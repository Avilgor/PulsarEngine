#include "Globals.h"
#include "Application.h"
#include "EngineResource.h"

#include <string>

EngineResource::EngineResource(RESOURCE_TYPE t)
{
	UUID = App->GenerateUUID_V4();
	type = t;
	resource = this;
	references = 0;
	currentPath = "Assets/";
	SetExtension();
}

EngineResource::EngineResource(RESOURCE_TYPE t, std::string uuid)
{
	UUID = uuid;
	type = t;
	resource = this;
	references = 0;
	currentPath = "Assets/";
	SetExtension();
}

EngineResource::~EngineResource()
{

}

void EngineResource::SetFullPath(std::string path)
{
	currentPath = path;
}

void EngineResource::SetAssetsPath(std::string path)
{
	currentPath = path;
	currentPath = currentPath.append(name);
	currentPath = currentPath.append(extension);
}

void EngineResource::SetExtension()
{
	switch (type)
	{
	case MESH_RES:
		extension = ".psmesh";
		break;
	case MATERIAL_RES:
		extension = ".psmaterial";
		break;
	case SCENE_RES:
		extension = ".psscene";
		break;
	}
	currentPath = currentPath.append(name);
	currentPath = currentPath.append(extension);
}