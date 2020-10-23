#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"


GameObject::GameObject(const char* n, GameObject* p)
{
	ID = App->GoIDNum;
	App->GoIDNum += 1;
	name = n;
	active = true;
	parent = p;
	AddComponent(TRANSFORM_COMP);
}


GameObject::~GameObject()
{
	delete transform;
	if (!Components.empty())
	{
		for (std::vector<Component*>::const_iterator it = Components.begin(); it != Components.end(); ++it)
		{
			delete (*it);
		}
		Components.clear();
	}

	if (!Childs.empty())
	{
		for (std::vector<GameObject*>::const_iterator it = Childs.begin(); it != Childs.end(); ++it)
		{
			delete (*it);
		}
		Childs.clear();
	}
}

void GameObject::SetActive(bool val)
{
	active = val;
}

void GameObject::UpdateTransform()
{
	transform->UpdateComponent();
	if (!Childs.empty())
	{
		for (std::vector<GameObject*>::iterator it = Childs.begin(); it != Childs.end(); ++it)
		{
			if ((*it)->active) (*it)->UpdateTransform();
		}
	}
}

void GameObject::UpdateGameObject()
{
	if (!Components.empty())
	{
		for (std::vector<Component*>::iterator it = Components.begin(); it != Components.end(); ++it)
		{
			if((*it)->active) (*it)->UpdateComponent();
		}
	}

	if (!Childs.empty())
	{
		for (std::vector<GameObject*>::iterator it = Childs.begin(); it != Childs.end(); ++it)
		{
			if ((*it)->active) (*it)->UpdateGameObject();
		}
	}
}

void GameObject::AddComponent(ComponentTypes type)
{
	switch (type)
	{
	case TRANSFORM_COMP:
		transform = new Transform(this);
		//LOG("Added transform component.");
		break;
	case MESH_COMP:
		Components.push_back((new Mesh(this))->GetComponent());
		//LOG("Added mesh component.");
		break;
	case MATERIAL_COMP:
		Components.push_back((new Material(this))->GetComponent());
		//LOG("Added material component.");
		break;
	default:
		break;
	}
	//LOG("Components number: %d", Components.size());
}

Component* GameObject::GetFirstComponentType(ComponentTypes type)
{
	Component* temp = nullptr;
	//LOG("Search type: %d",type);
	if (!Components.empty())
	{
		//LOG("Components not empty: %d",Components.size());
		for (std::vector<Component*>::iterator it = Components.begin(); it != Components.end(); it++)
		{
			//LOG("Component type: %d", (*it)->compType);
			if ((*it)->compType == type)
			{
				//LOG("Found component.");
				temp = (*it);
				break;
			}
		}
	}
	return temp;
}

void GameObject::DeleteGOComponent(ComponentTypes type)
{
	if (!Components.empty())
	{		
		for (std::vector<Component*>::iterator it = Components.begin(); it != Components.end(); it++)
		{
			if ((*it)->compType == type)
			{
				(*it)->DeleteComponent();
				Components.erase(it);
				delete (*it);
				break;
			}
		}
	}
}

void GameObject::AddChild(GameObject* child)
{
	child->parent = this;
	Childs.push_back(child);
}

void GameObject::CreateChild()
{
	GameObject* child = new GameObject("Child",this);
	Childs.push_back(child);
}

void GameObject::DeleteChild(int id)
{
	if (!Childs.empty())
	{		
		for (std::vector<GameObject*>::iterator it = Childs.begin(); it != Childs.end(); it++)
		{
			if ((*it)->ID == id)
			{			
				(*it)->DeleteGameobject();
				Childs.erase(it);
				delete (*it);				
				break;
			}
		}
	}			
}	

void GameObject::RemoveChild(int id)
{
	if (!Childs.empty())
	{
		for (std::vector<GameObject*>::iterator it = Childs.begin(); it != Childs.end(); it++)
		{
			if ((*it)->ID == id)
			{
				Childs.erase(it);
				break;
			}
		}
	}
}

void GameObject::DeleteGameobject()
{
	transform->DeleteComponent();
	if (!Components.empty())
	{
		for (std::vector<Component*>::iterator it = Components.begin(); it != Components.end(); ++it)
		{
			(*it)->DeleteComponent();
			//delete (*it);
		}
	}

	if (!Childs.empty())
	{
		for (std::vector<GameObject*>::iterator it = Childs.begin(); it != Childs.end(); ++it)
		{
			(*it)->DeleteGameobject();
			//delete (*it);
		}
	}
}