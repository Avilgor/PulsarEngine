#ifndef __Camera_H__
#define __Camera_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

#pragma comment (lib, "MathGeoLib/libx86/MathGeoLib.lib") 


class Camera : public Component
{
public:

	Camera(GameObject* parent);
	~Camera();

	void UpdateComponent();
	void DeleteComponent();
	void SaveComponent(JSonHandler* file);
	void LoadComponent(JSonHandler* file);

public:
	
};

#endif //__Camera_H__