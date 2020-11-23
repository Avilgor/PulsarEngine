#ifndef __Material_H__
#define __Material_H__

#include "Globals.h"
#include "Color.h"
#include "Component.h"
#include "RES_Material.h"

#include <vector>

class JSonHandler;

class Material : public Component
{
public:

	Material(GameObject* parent);
	~Material();

	void UpdateComponent();
	void DeleteComponent();
	RES_Material* GetMaterial() { return resMaterial; }
	void SetMaterial(RES_Material* mat);
	void DeleteMaterial();
	void SaveComponent(JSonHandler* file);
	void LoadComponent(JSonHandler* file);

private:
	
	RES_Material* resMaterial = nullptr;
};

#endif //__Material_H__