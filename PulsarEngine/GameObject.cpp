#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "JSonHandler.h"
#include "PhysBody3D.h"
#include "RigidBody.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "ConstraintPoint.h"
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
	transformUpdate = true;
	CreateTransform();
	hasAABB = false;
	tag = DEFAULT_TAG;
	layer = DEFAULT_LAYER;
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
	tag = DEFAULT_TAG;
	layer = DEFAULT_LAYER;
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
	tag = DEFAULT_TAG;
	layer = DEFAULT_LAYER;
}

GameObject::~GameObject()
{
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
	if (active)
	{
		if (!toDelete)
		{			
			if (camera != nullptr) camera->UpdateComponent();

			if (!Components.empty())
			{
				for (std::vector<Component*>::iterator it = Components.begin(); it != Components.end(); ++it)
				{
					if ((*it)->active && (*it)->compType != MESH_COMP) (*it)->UpdateComponent();
				}
			}

			if (boxcollider != nullptr) boxcollider->UpdateComponent();
			if (capsulecollider != nullptr) capsulecollider->UpdateComponent();
			if (spherecollider != nullptr) spherecollider->UpdateComponent();
			if (pointconstraint != nullptr) pointconstraint->UpdateComponent();

			if (!Childs.empty())
			{
				for (std::vector<GameObject*>::iterator it = Childs.begin(); it != Childs.end(); ++it)
				{
					(*it)->UpdateGameObject();
				}
			}
		}
	}

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
	if (transform != nullptr) transform->SaveComponent(&node);
	if (camera != nullptr) camera->SaveComponent(&node);
	if (!Components.empty())
	{
		for (std::vector<Component*>::iterator it = Components.begin(); it != Components.end(); ++it)
		{
			(*it)->SaveComponent(&node);
		}
	}
	if (boxcollider != nullptr) boxcollider->SaveComponent(&node);
	if (capsulecollider != nullptr) capsulecollider->SaveComponent(&node);
	if (spherecollider != nullptr) spherecollider->SaveComponent(&node);
	if (pointconstraint != nullptr) pointconstraint->SaveComponent(&node);

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

	//Load components	
	file->LoadArray("Components");
	int num = file->GetArrayCount("Components");
	if (num > 0)
	{
		Component* comp = nullptr;
		for (int i = 0; i < num; i++)
		{
			JSonHandler json = file->GetNodeArray("Components", i);
			switch ((int)json.GetNum("CompType"))
			{
			case TRANSFORM_COMP:
				transform->LoadComponent(&json);
				break;
			case MESH_COMP:
				comp = AddComponent(MESH_COMP);
				if (comp != nullptr)
				{
					if(comp->AsMesh() != nullptr) comp->AsMesh()->LoadComponent(&json);
				}
				break;
			case MATERIAL_COMP:
				comp = AddComponent(MATERIAL_COMP);
				if (comp != nullptr)
				{
					if(comp->AsMaterial() != nullptr) comp->AsMaterial()->LoadComponent(&json);
				}
				break;
			case CAMERA_COMP:
				comp = AddComponent(CAMERA_COMP);
				if (comp != nullptr)
				{
					if(comp->AsCamera() != nullptr) comp->AsCamera()->LoadComponent(&json);
				}
				break;
			case BOX_COLLIDER_COMP:
				comp = AddComponent(BOX_COLLIDER_COMP);
				if (comp != nullptr)
				{
					if (comp->AsBoxCollider() != nullptr) comp->AsBoxCollider()->LoadComponent(&json);
				}
				break;
			case SPHERE_COLLIDER_COMP:
				comp = AddComponent(SPHERE_COLLIDER_COMP);
				if (comp != nullptr)
				{
					if (comp->AsSphereCollider() != nullptr) comp->AsSphereCollider()->LoadComponent(&json);
				}
				break;
			case CAPSULE_COLLIDER_COMP:
				comp = AddComponent(CAPSULE_COLLIDER_COMP);
				if (comp != nullptr)
				{
					if (comp->AsCapsuleCollider() != nullptr) comp->AsCapsuleCollider()->LoadComponent(&json);
				}
				break;
			case CONSTRAINT_POINT_COMP:
				comp = AddComponent(CONSTRAINT_POINT_COMP);
				if (comp != nullptr)
				{
					if (comp->AsPointConstraint() != nullptr) comp->AsPointConstraint()->LoadComponent(&json);
				}
				break;
			/*case CONSTRAINT_HINGE_COMP:
				comp = AddComponent(CONSTRAINT_HINGE_COMP);
				if (comp != nullptr)
				{
					if (comp->AsCapsuleCollider() != nullptr) comp->AsCapsuleCollider()->LoadComponent(&json);
				}
				break;
			case CONSTRAINT_SLIDER_COMP:
				comp = AddComponent(CONSTRAINT_SLIDER_COMP);
				if (comp != nullptr)
				{
					if (comp->AsCapsuleCollider() != nullptr) comp->AsCapsuleCollider()->LoadComponent(&json);
				}
				break;
			case CONSTRAINT_CONE_COMP:
				comp = AddComponent(CONSTRAINT_CONE_COMP);
				if (comp != nullptr)
				{
					if (comp->AsCapsuleCollider() != nullptr) comp->AsCapsuleCollider()->LoadComponent(&json);
				}
				break;*/
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

		if (hasAABB)
		{
			Component* temp = GetFirstComponentType(MESH_COMP);
			if (temp != nullptr)
			{
				Mesh* mesh = temp->AsMesh();
				if (mesh != nullptr)
				{
					AABBCheck temp = App->camera->CheckAABB(Gaabb);
					if (temp == AABB_IN || temp == AABB_INTERSECT)
					{
						App->camera->AddDrawnGameobject(this);
						mesh->UpdateComponent();
					}
				}
			}
		}
		
		if (!Childs.empty())
		{
			for (std::vector<GameObject*>::iterator it = Childs.begin(); it != Childs.end(); ++it)
			{
				(*it)->DrawMesh();
			}
		}
	}	
}

bool GameObject::HasCollider()
{
	if (boxcollider != nullptr || capsulecollider != nullptr || spherecollider != nullptr)
	{
		return true;
	}
	else return false;
}

std::vector<Component*> GameObject::GetAllComponentsByType(ComponentTypes type)
{
	if (!Components.empty())
	{
		std::vector<Component*> temp;
		for (int i = 0; i < Components.size(); i++)
		{
			if (Components[i]->compType == type)
			{
				temp.push_back(Components[i]);
			}
		}
		return temp;
	}
	else return Components;
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
					if (ray.Intersects(Gaabb)) return this;				
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
					if (ray.Intersects(mesh->GetMeshAABB())) vec->push_back(this);				
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
		else hasAABB = false;
	}
	else hasAABB = false;
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
	BoxCollider* boxColl = nullptr;
	SphereCollider* sphereColl = nullptr;
	CapsuleCollider* capsuleColl = nullptr;
	ConstraintPoint* pointconst = nullptr;
	switch (type)
	{
	case MESH_COMP:
		mesh = new Mesh(this);
		Components.push_back(mesh->component);
		return mesh->component;
		break;
	case MATERIAL_COMP:
		mat = new Material(this);
		Components.push_back(mat->component);
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
		break;
	case BOX_COLLIDER_COMP:
		if (spherecollider != nullptr) DeleteGOComponent(SPHERE_COLLIDER_COMP);
		if (capsulecollider != nullptr) DeleteGOComponent(CAPSULE_COLLIDER_COMP);
		boxColl = new BoxCollider(this);
		boxcollider = boxColl;
		//Components.push_back(boxColl->component);
		return boxColl->component;		
		break;
	case SPHERE_COLLIDER_COMP:
		if (boxcollider != nullptr) DeleteGOComponent(BOX_COLLIDER_COMP);
		if (capsulecollider != nullptr) DeleteGOComponent(CAPSULE_COLLIDER_COMP);
		sphereColl = new SphereCollider(this);
		spherecollider = sphereColl;
		//Components.push_back(sphereColl->component);
		return sphereColl->component;	
		break;
	case CAPSULE_COLLIDER_COMP:
		if (spherecollider != nullptr) DeleteGOComponent(SPHERE_COLLIDER_COMP);
		if (boxcollider != nullptr) DeleteGOComponent(BOX_COLLIDER_COMP);
		capsuleColl = new CapsuleCollider(this);
		capsulecollider = capsuleColl;
		//Components.push_back(capsuleColl->component);
		return capsuleColl->component;
		break;
	case CONSTRAINT_POINT_COMP:
		if (HasCollider())
		{
			if (pointconstraint != nullptr)DeleteGOComponent(CONSTRAINT_POINT_COMP);
			pointconst = new ConstraintPoint(this);
			pointconstraint = pointconst;
			return pointconst->component;
		}
		else return nullptr;
		break;
	/*case CONSTRAINT_HINGE_COMP:
		if (HasCollider())
		{
			if (pointconstraint != nullptr)DeleteGOComponent(CONSTRAINT_POINT_COMP);
			capsuleColl = new CapsuleCollider(this);
			pointconstraint = pointconst;
			return pointconst->component;
		}
		else return nullptr;
		break;
	case CONSTRAINT_SLIDER_COMP:
		if (HasCollider())
		{
			if (pointconstraint != nullptr)DeleteGOComponent(CONSTRAINT_POINT_COMP);
			capsuleColl = new CapsuleCollider(this);
			pointconstraint = pointconst;
			return pointconst->component;
		}
		else return nullptr;
		break;
	case CONSTRAINT_CONE_COMP:
		if (HasCollider())
		{
			if (pointconstraint != nullptr)DeleteGOComponent(CONSTRAINT_POINT_COMP);
			capsuleColl = new CapsuleCollider(this);
			pointconstraint = pointconst;
			return pointconst->component;
		}
		else return nullptr;
		break;*/
	default:
		return comp;
		break;
	}
}

Component* GameObject::GetColliderComp()
{
	if (boxcollider != nullptr) return boxcollider;
	else if (spherecollider != nullptr) return spherecollider;
	else if (capsulecollider != nullptr) return capsulecollider;
	return nullptr;
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

float4x4 GameObject::GetGlobalTransform()
{
	return transform->GetGlobalTransform();
}

float4x4 GameObject::GetLocalTransform()
{
	return transform->GetTransform();
}

void GameObject::DeleteGOComponent(ComponentTypes type)
{
	if (type == CAMERA_COMP && camera != nullptr)
	{
		camera->DeleteComponent();
		delete camera;
		camera = nullptr;
	}
	else if (type == BOX_COLLIDER_COMP && boxcollider != nullptr)
	{
		boxcollider->DeleteComponent();
		delete boxcollider;
		boxcollider = nullptr;
	}
	else if (type == SPHERE_COLLIDER_COMP && spherecollider != nullptr)
	{
		spherecollider->DeleteComponent();
		delete spherecollider;
		spherecollider = nullptr;
	}
	else if (type == CAPSULE_COLLIDER_COMP && capsulecollider != nullptr)
	{
		capsulecollider->DeleteComponent();
		delete capsulecollider;
		capsulecollider = nullptr;
	}
	else if (type == CONSTRAINT_POINT_COMP && pointconstraint != nullptr)
	{
		pointconstraint->DeleteComponent();
		delete pointconstraint;
		pointconstraint = nullptr;
	}
	else if (type == CONSTRAINT_HINGE_COMP && boxcollider != nullptr)
	{
		capsulecollider->DeleteComponent();
		delete capsulecollider;
		capsulecollider = nullptr;
	}
	else if (type == CONSTRAINT_SLIDER_COMP && boxcollider != nullptr)
	{
		capsulecollider->DeleteComponent();
		delete capsulecollider;
		capsulecollider = nullptr;
	}
	else if (type == CONSTRAINT_CONE_COMP && boxcollider != nullptr)
	{
		capsulecollider->DeleteComponent();
		delete capsulecollider;
		capsulecollider = nullptr;
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
			UpdateAABB();
		}
	}	
}

void GameObject::DeleteGOComponent(std::string uuid)
{
	if (!Components.empty())
	{
		std::vector<Component*> temp;
		for (std::vector<Component*>::iterator it = Components.begin(); it != Components.end(); it++)
		{
			if ((*it)->UUID == uuid)(*it)->DeleteComponent();		
			else temp.push_back((*it));
		}
		Components.clear();
		Components = temp;
		temp.clear();
		UpdateAABB();
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
			//if (!CheckIfGotParentUUID(p->UUID))
			//{
				parent->RemoveChild(UUID);
				parent = p;
				parentUUID = p->UUID;
				parent->AddChild(this);
			//}
		}
		else
		{
			parentUUID = p->UUID;
			parent = p;
		}
		transform->SetGlobalTransform();
	}
}

/*bool GameObject::CheckIfGotParentUUID(std::string id)
{
	bool ret = false;
	if (parent != nullptr)
	{
		if (parent->UUID.compare(id) != 0)
		{
			if (!Childs.empty())
			{
				for (int i = 0; i < Childs.size(); i++)
				{
					ret = Childs[i]->CheckIfGotParentUUID(id);
					if (ret) break;
				}
			}
		}
		else ret = true;
	}
	return ret;
}*/

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
	delete this;
}

void GameObject::SaveToDelete(GameObject* trash)
{
	RemoveChild(trash->UUID);
	toDeleteChilds.push_back(trash);
}

void GameObject::DeleteGameobject()
{	
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

void GameObject::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{

}