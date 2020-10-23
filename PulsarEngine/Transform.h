#ifndef __Transform_H__
#define __Transform_H__

#include "Component.h"

#include <vector>


class Transform : public Component
{
public:

	Transform(GameObject* parent);
	~Transform();

	void UpdateComponent();
	void DeleteComponent();

public:

};

#endif //__Transform_H__