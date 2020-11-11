#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "JSonHandler.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/MathGeoLib.h"


GameObject::GameObject(const char* n, GameObject* p)
{
	UUID = App->GenerateUUID_V4();
	name = n;
	active = true;
	selected = false;
	showHierarchy = false;
	toDelete = false;
	parent = p;
	if(p != nullptr) parentUUID = p->UUID;
	transformUpdate = false;
	CreateTransform();
	hasAABB = false;
	//LOG("Created gameobject %s",name.c_str());
}

GameObject::GameObject(const char* n, float3 pos, Quat rotation, float3 scale, GameObject* p)
{
	UUID = App->GenerateUUID_V4();
	name = n;
	active = true;
	selected = false;
	showHierarchy = false;
	toDelete = false;
	parent = p;
	if (p != nullptr) parentUUID = p->UUID;
	transformUpdate = false;
	CreateTransform(pos,rotation,scale);
	hasAABB = false;
}

GameObject::GameObject(const char* n, float3 pos, float3 rotation, float3 scale, GameObject* p)
{
	UUID = App->GenerateUUID_V4();
	name = n;
	active = true;
	selected = false;
	showHierarchy = false;
	toDelete = false;
	parent = p;
	if (p != nullptr) parentUUID = p->UUID;
	transformUpdate = false;
	CreateTransform(pos, rotation, scale);
	hasAABB = false;
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

void GameObject::ToggleUpdateTransform()
{
	if(transform != nullptr) transform->updateTransform = true;

	if (!Childs.empty())
	{
		for (std::vector<GameObject*>::iterator it = Childs.begin(); it != Childs.end(); ++it)
		{
			(*it)->ToggleUpdateTransform();
		}
	}
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
	node.CreateArray("Components");
	if(transform != nullptr) transform->SaveComponent(&node);
	if (!Components.empty())
	{
		for (std::vector<Component*>::iterator it = Components.begin(); it != Components.end(); ++it)
		{
			(*it)->SaveComponent(&node);
		}
	}

	//Call all childs save method
	if (!Childs.empty())
	{
		for (std::vector<GameObject*>::iterator it = Childs.begin(); it != Childs.end(); ++it)
		{
			(*it)->SaveGameobject(file, label);
		}
	}
}

void GameObject::LoadGameObject(JSonHandler* file)
{
	//Gameobject stats
	UUID = file->GetString("UUID");
	parentUUID = file->GetString("Parent_UUID");
	name = file->GetString("name");
	active = file->GetBool("active");

	//LOG("Loading %s gameobject components...",name.c_str());
	//Load components	
	file->LoadArray("Components");
	int num = file->GetArrayCount("Components");
	if (num > 0)
	{
		Component* comp = nullptr;
		for (int i = 0; i < num; i++)
		{
			JSonHandler json = file->GetNodeArray("Components", i);
			//LOG("Comp type: %d",num);
			switch ((int)json.GetNum("CompType"))
			{
			case TRANSFORM_COMP:
				//LOG("Load transform");
				transform->LoadComponent(&json);
				break;
			case MESH_COMP:
				//LOG("Load mesh");
				comp = AddComponent(MESH_COMP);
				if (comp != nullptr) comp->AsMesh()->LoadComponent(&json);
				break;
			case MATERIAL_COMP:
				//LOG("Load material");
				comp = AddComponent(MATERIAL_COMP);
				if (comp != nullptr) comp->AsMaterial()->LoadComponent(&json);
				break;
			case CAMERA_COMP:
				//LOG("Load camera");
				comp = AddComponent(CAMERA_COMP);
				if (comp != nullptr) comp->AsCamera()->LoadComponent(&json);
				break;
			}	
		}
	}
}

void GameObject::LoadTransform(JSonHandler* file)
{
	file->LoadArray("Components");
	JSonHandler json = file->GetNodeArray("Components", 0);
	transform->LoadComponent(&json);
}

void GameObject::DrawMesh()
{
	if (active)
	{
		if (hasAABB && drawAABB)
		{
			App->renderer3D->RenderAABB(Gaabb);
		}

		//Disabled because frustum culling is not working well
		/*Component* temp = GetFirstComponentType(MESH_COMP);
		if (temp != nullptr)
		{
			Mesh* mesh = temp->AsMesh();
			if (mesh != nullptr)
			{
				AABBCheck temp = App->camera->CheckAABB(Gaabb);
				if (temp == AABB_IN || temp == AABB_INTERSECT) mesh->UpdateComponent();
				//else LOG("Gameobject %s mesh is not visible.",name.c_str());
			}
		}*/
		if (hasAABB)
		{
			AABBCheck temp = App->camera->CheckAABB(Gaabb);
			if (temp == AABB_IN || temp == AABB_INTERSECT)
			{
				if (!Components.empty())
				{
					for (std::vector<Component*>::iterator it = Components.begin(); it != Components.end(); ++it)
					{
						if ((*it)->active && (*it)->compType == MESH_COMP) (*it)->UpdateComponent();
					}
				}
			}
			//else LOG("Gameobject %s out of frustum.", name.c_str());
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

GameObject* GameObject::CheckRayIntersect(LineSegment ray)
{
	if (active)
	{
		if (hasAABB)
		{
			Component* temp = GetFirstComponentType(MESH_COMP);
			if (temp != nullptr)
			{
				Mesh* mesh = temp->AsMesh();
				if (mesh != nullptr)
				{
					if (ray.Intersects(mesh->GetMeshAABB()))
					{
						LOG("Gameobject %s mouse click intersect.", name.c_str());
						return this;
					}
				}
			}
		}
	}

	return nullptr;
}

void GameObject::CheckRayIntersect(std::vector<GameObject*>* vec, LineSegment ray)
{
	if (active)
	{
		if (hasAABB)
		{
			Component* temp = GetFirstComponentType(MESH_COMP);
			if (temp != nullptr)
			{
				Mesh* mesh = temp->AsMesh();
				if (mesh != nullptr)
				{
					if (ray.Intersects(mesh->GetMeshAABB()))
					{
						LOG("Gameobject %s mouse click intersect.", name.c_str());
						vec->push_back(this);
					}
				}
			}
		}

		if (!Childs.empty())
		{
			for (std::vector<GameObject*>::iterator it = Childs.begin(); it != Childs.end(); ++it)
			{
				(*it)->CheckRayIntersect(vec,ray);
			}
		}
	}
}

void GameObject::SetDrawAABB(bool val)
{
	drawAABB = val;
	if (!Childs.empty())
	{
		for (std::vector<GameObject*>::iterator it = Childs.begin(); it != Childs.end(); ++it)
		{
			(*it)->SetDrawAABB(val);
		}
	}
}

void GameObject::UpdateAABB()
{
	Component* temp = GetFirstComponentType(MESH_COMP);
	if (temp != nullptr)
	{
		Mesh* mesh = temp->AsMesh();
		if (mesh != nullptr)
		{
			Gobb = mesh->GetMeshAABB();
			Gobb.Transform(transform->GetGlobalTransform());			
			Gaabb.SetNegativeInfinity();
			Gaabb.Enclose(Gobb);
			hasAABB = true;
		}
	}	
}

AABB GameObject::GetGlobalAABB()
{
	return Gaabb;
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


Component* GameObject::AddComponent(ComponentTypes type)
{
	Component* comp = nullptr;
	Mesh* mesh = nullptr;
	Material* mat = nullptr;
	switch (type)
	{
	case MESH_COMP:
		mesh = new Mesh(this);
		Components.push_back(mesh->component);
		//LOG("Added component mesh");
		return mesh->component;
		break;
	case MATERIAL_COMP:
		mat = new Material(this);
		Components.push_back(mat->component);
		//LOG("Added component material");
		return mat->component;
		break;
	case CAMERA_COMP:
		if (camera != nullptr)
		{
			camera->DeleteComponent();
			delete camera;
			camera = nullptr;
		}
		camera = new Camera(this);
		return camera->component;
		//LOG("Added component camera");
		break;
	default:
		return comp;
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
	if (type == CAMERA_COMP && camera != nullptr)
	{
		camera->DeleteComponent();
		delete camera;
		camera = nullptr;
	}
	else if (type != TRANSFORM_COMP)
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
	toAddChilds.push_back(child);
	child->SetParent(this);
}

GameObject* GameObject::CreateChild()
{
	GameObject* child = new GameObject("Child",this);
	toAddChilds.push_back(child);
	return child;
}

GameObject* GameObject::CreateChild(const char* n)
{
	GameObject* child = new GameObject(n, this);
	toAddChilds.push_back(child);
	return child;
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
	if (p != nullptr)
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
		transform->SetGlobalTransform();
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