#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "JSonHandler.h"
#include "MathGeoLib/include/MathGeoLib.h"


GameObject::GameObject(const char* n, GameObject* p)
{
	UUID = App->GenerateUUID_V4();
	//App->GoIDNum += 1;
	name = n;
	active = true;
	selected = false;
	showHierarchy = false;
	toDelete = false;
	parent = p;
	if(p != nullptr) parentUUID = p->UUID;
	transformUpdate = false;
	CreateTransform();
	//AddComponent(TRANSFORM_COMP);
}

GameObject::GameObject(const char* n, float3 pos, Quat rotation, float3 scale, GameObject* p)
{
	UUID = App->GenerateUUID_V4();
	//App->GoIDNum += 1;
	name = n;
	active = true;
	selected = false;
	showHierarchy = false;
	toDelete = false;
	parent = p;
	if (p != nullptr) parentUUID = p->UUID;
	transformUpdate = false;
	CreateTransform(pos,rotation,scale);
	//LOG("ID: %s", UUID.c_str());
	//AddComponent(TRANSFORM_COMP);
}

GameObject::GameObject(const char* n, float3 pos, float3 rotation, float3 scale, GameObject* p)
{
	UUID = App->GenerateUUID_V4();
	//App->GoIDNum += 1;
	name = n;
	active = true;
	selected = false;
	showHierarchy = false;
	toDelete = false;
	parent = p;
	if (p != nullptr) parentUUID = p->UUID;
	transformUpdate = false;
	CreateTransform(pos, rotation, scale);
	//LOG("ID: %s", UUID.c_str());
	//AddComponent(TRANSFORM_COMP);
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
	if (!Childs.empty())
	{
		for (std::vector<GameObject*>::iterator it = Childs.begin(); it != Childs.end(); ++it)
		{
			(*it)->SetActive(val);
		}
	}
}

void GameObject::SetUUID(const char* id)
{
	UUID = id;
}

void GameObject::UpdateTransform()
{
	if (!toDelete)
	{
		transform->UpdateComponent();			

		if (!Childs.empty())
		{
			for (std::vector<GameObject*>::iterator it = Childs.begin(); it != Childs.end(); ++it)
			{
				(*it)->UpdateTransform();
			}
		}
	}
}

void GameObject::UpdateGameObject()
{	
	if (!toDelete)
	{
		if (!toDeleteChilds.empty())
		{
			for (std::vector<GameObject*>::iterator it = toDeleteChilds.begin(); it != toDeleteChilds.end(); ++it)
			{
				(*it)->Delete();
			}
			toDeleteChilds.clear();
		}
	
		if (!toAddChilds.empty())
		{
			AddPendingChilds();			
		}

		if (!Components.empty())
		{
			for (std::vector<Component*>::iterator it = Components.begin(); it != Components.end(); ++it)
			{
				if ((*it)->active && (*it)->compType != MESH_COMP) (*it)->UpdateComponent();				
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
}

void GameObject::UpdateAABB()
{

}

void GameObject::SaveGameobject(JSonHandler* file, const char* label)
{
	JSonHandler node = file->InsertNodeArray(label);

	//Gameobject stats
	node.SaveString("UUID",UUID.c_str());
	node.SaveString("Parent_UUID", parentUUID.c_str());
	node.SaveString("name", name.c_str());
	node.SaveBool("active", active);

	//Save childs UUID
	node.CreateArray("Childs");	
	if (!Childs.empty())
	{
		for (std::vector<GameObject*>::iterator it = Childs.begin(); it != Childs.end(); ++it)
		{
			node.InsertStringArray("Childs", (*it)->UUID.c_str());
			//(*it)->SaveGameobject(file,label);
		}
	}

	//Save components	
	JSonHandler comp = node.CreateNode("Components");
	if(transform != nullptr) transform->SaveComponent(&comp);
	if (!Components.empty())
	{
		for (std::vector<Component*>::iterator it = Components.begin(); it != Components.end(); ++it)
		{
			(*it)->SaveComponent(&comp);
		}
	}

	//Call all childs save method
	if (!Childs.empty())
	{
		for (std::vector<GameObject*>::iterator it = Childs.begin(); it != Childs.end(); ++it)
		{
			//node.InsertStringArray("Childs", (*it)->UUID.c_str());
			(*it)->SaveGameobject(file, label);
		}
	}
}

void GameObject::LoadGameObject(JSonHandler* file, const char* label)
{

}

void GameObject::DrawMesh()
{
	if (active)
	{
		if (!Components.empty())
		{
			for (std::vector<Component*>::iterator it = Components.begin(); it != Components.end(); ++it)
			{
				if ((*it)->active && (*it)->compType == MESH_COMP) (*it)->UpdateComponent();
			}
		}

		if (!Childs.empty())
		{
			for (std::vector<GameObject*>::iterator it = Childs.begin(); it != Childs.end(); ++it)
			{
				if ((*it)->active) (*it)->DrawMesh();
			}
		}
	}	
}

void GameObject::CreateTransform()
{
	transform = new Transform(this);
}

void GameObject::CreateTransform(float3 pos, Quat rot, float3 scale)
{
	transform = new Transform(this,pos,rot,scale);
}

void GameObject::CreateTransform(float3 pos, float3 rot, float3 scale)
{
	transform = new Transform(this, pos, rot, scale);
}


void GameObject::AddComponent(ComponentTypes type)
{
	switch (type)
	{
	/*case TRANSFORM_COMP:
		transform = new Transform(this);
		break;*/
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
	if (type != TRANSFORM_COMP)
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
}

void GameObject::AddChild(GameObject* child)
{
	child->SetParent(this);
	toAddChilds.push_back(child);
}

void GameObject::CreateChild()
{
	GameObject* child = new GameObject("Child",this);
	toAddChilds.push_back(child);
}

void GameObject::AddPendingChilds()
{
	for (std::vector<GameObject*>::iterator it = toAddChilds.begin(); it != toAddChilds.end(); ++it)
	{
		Childs.push_back((*it));
	}	
	toAddChilds.clear();
}

void GameObject::DeleteChild(std::string id)
{
	if (!Childs.empty())
	{		
		std::vector<GameObject*> temp;
		for (std::vector<GameObject*>::iterator it = Childs.begin(); it != Childs.end(); it++)
		{
			if ((*it)->UUID.compare(id) == 0) (*it)->DeleteGameobject();
			else temp.push_back((*it));
		}
		Childs.clear();
		Childs = temp;
		temp.clear();
	}			
}	

void GameObject::DeleteAllChilds()
{
	if (!Childs.empty())
	{
		std::vector<GameObject*> temp = Childs;
		for (std::vector<GameObject*>::iterator it = temp.begin(); it != temp.end(); it++)
		{
			(*it)->DeleteGameobject();
		}
		Childs.clear();
	}
}

void GameObject::RemoveChild(std::string id)
{
	if (!Childs.empty())
	{
		std::vector<GameObject*> temp;
		for (std::vector<GameObject*>::iterator it = Childs.begin(); it != Childs.end(); it++)
		{
			if ((*it)->UUID.compare(id) != 0) temp.push_back((*it));			
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

void GameObject::SetParent(GameObject* p)
{
	if (parent != nullptr && p->UUID.compare(parent->UUID) != 0)
	{
		parent->RemoveChild(UUID);
		parent = p;
		parentUUID = p->UUID;
		parent->AddChild(this);
	}
	else
	{
		parentUUID = p->UUID;
		parent = p;
	}
}

void GameObject::Delete()
{
	if (!toDeleteChilds.empty())
	{
		std::vector<GameObject*> toDelete = toDeleteChilds;
		for (std::vector<GameObject*>::iterator it = toDelete.begin(); it != toDelete.end(); ++it)
		{
			(*it)->Delete();
		}
		toDelete.clear();
	}

	if (!Components.empty())
	{
		for (std::vector<Component*>::iterator it = Components.begin(); it != Components.end(); ++it)
		{
			(*it)->DeleteComponent();
		}
		Components.clear();
	}

	transform->DeleteComponent();
	transform = nullptr;
	LOG("Deleted Gameobject: %s", name.c_str());
	delete this;
}

void GameObject::SaveToDelete(GameObject* trash)
{
	RemoveChild(trash->UUID);
	toDeleteChilds.push_back(trash);
}

void GameObject::DeleteGameobject()
{	
	LOG("To delete: %s",name.c_str());
	toDelete = true;
	if(parent != nullptr) parent->SaveToDelete(this);
	if (!Childs.empty())
	{
		std::vector<GameObject*> toDelete = Childs;
		for (std::vector<GameObject*>::iterator it = toDelete.begin(); it != toDelete.end(); ++it)
		{
			(*it)->DeleteGameobject();
		}
		toDelete.clear();
	}
}