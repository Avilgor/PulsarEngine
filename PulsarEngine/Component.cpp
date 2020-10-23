#include "Globals.h"
#include "Component.h"

Component::Component(GameObject* parent, ComponentTypes type)
{
	gameobject = parent;
	active = true;
	compType = type;
	component = this;
}

Component::~Component()
{}

void Component::SetActive(bool val)
{
	active = val;
}