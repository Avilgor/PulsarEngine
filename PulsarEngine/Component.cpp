#include "Globals.h"
#include "Application.h"
#include "Component.h"

Component::Component(GameObject* parent, ComponentTypes type)
{
	gameobject = parent;
	active = true;
	compType = type;
	component = this;
	UUID = App->GenerateUUID_V4();
}

Component::~Component()
{
}

void Component::SetActive(bool val)
{
	active = val;
}