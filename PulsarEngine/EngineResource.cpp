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
}

EngineResource::EngineResource(RESOURCE_TYPE t, std::string uuid)
{
	UUID = uuid;
	type = t;
	resource = this;
	references = 0;
}

EngineResource::~EngineResource()
{

}