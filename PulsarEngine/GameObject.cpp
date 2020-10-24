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
	selected = false;
	showHierarchy = false;
	parent = p;
	AddComponent(TRANSFORM_COMP);
}


GameObject::~GameObject()
{
	//delete transform;
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
	if(transform != nullptr) transform->UpdateComponent();
	if (!Childs.empty())
	{
		for (std::vector<GameObject*>::iterator it = Childs.begin(); it != Childs.end(); ++it)
		{
			if (transform->needUpdate) (*it)->transform->needUpdate = true;
			(*it)->UpdateTransform();
		}
	}
}

void GameObject::UpdateGameObject()
{
	//LOG("Update Gameobject: %s",name.c_str());
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
		break;
	case MESH_COMP:
		Components.push_back((new Mesh(this))->GetComponent());
		break;
	case MATERIAL_COMP:
		Components.push_back((new Material(this))->GetComponent());
		break;
	default:
		break;
	}
}

Component* GameObject::GetFirstComponentType(ComponentTypes type)
{
	Component* temp = nullptr;
	if (!Components.empty())
	{
		for (std::vector<Component*>::iterator it = Components.begin(); it != Components.end(); it++)
		{
			if ((*it)->compType == type)
			{
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
		std::vector<Component*> temp;
		for (std::vector<Component*>::iterator it = Components.begin(); it != Components.end(); it++)
		{
			if ((*it)->compType == type) (*it)->DeleteComponent();			
			else temp.push_back((*it));
		}
		Components.clear();
		Components = temp;
		temp.clear();
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
		std::vector<GameObject*> temp;
		for (std::vector<GameObject*>::iterator it = Childs.begin(); it != Childs.end(); it++)
		{
			if ((*it)->ID == id) (*it)->DeleteGameobject();
			else temp.push_back((*it));
		}
		Childs.clear();
		Childs = temp;
		temp.clear();
	}			
}	

void GameObject::RemoveChild(int id)
{
	if (!Childs.empty())
	{
		std::vector<GameObject*> temp;
		for (std::vector<GameObject*>::iterator it = Childs.begin(); it != Childs.end(); it++)
		{
			if ((*it)->ID != id) temp.push_back((*it));			
		}
		Childs.clear();
		Childs = temp;
		temp.clear();
	}
}

bool GameObject::HasChilds()
{
	if (Childs.empty()) return false;
	else return true;
}


void GameObject::DeleteGameobject()
{	
	LOG("Delete Gameobject: %s",name.c_str());

	if (!Childs.empty())
	{
		std::vector<GameObject*> toDelete = Childs;
		for (std::vector<GameObject*>::iterator it = toDelete.begin(); it != toDelete.end(); ++it)
		{
			(*it)->DeleteGameobject();
			//Childs.erase(it);
			//delete (*it);
		}
		toDelete.clear();
		//Childs.clear();
	}

	if (!Components.empty())
	{
		//LOG("To delete components: %d",Components.size());
		for (std::vector<Component*>::iterator it = Components.begin(); it != Components.end(); ++it)
		//for (int i = 0; i < Components.size();i++)
		{
			//LOG("Delete component: %d", Components[i].compType);
			(*it)->DeleteComponent();
			//Components[i]->DeleteComponent();
			//Components.erase(it);
			//delete (*it);
		}	
		Components.clear();
	}	

	transform->DeleteComponent();
	transform = nullptr;
	if(parent != nullptr) parent->RemoveChild(ID);
	delete this;
}