#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "GameObject.h"
#include "UI_Element.h"

class Boundary
{
public:
	Boundary();
	virtual ~Boundary();
	virtual bool CheckCollision(GameObject* object, GameObject* player);
	virtual bool CheckCollision2D(UI_Element* object, Vector3 point);
};








#endif