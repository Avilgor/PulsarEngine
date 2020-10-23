#ifndef __Material_H__
#define __Material_H__

#include "Component.h"


class Material : public Component
{
public:

	Material(GameObject* parent);
	~Material();

	void UpdateComponent();
	void DeleteComponent();

public:

};

#endif //__Material_H__