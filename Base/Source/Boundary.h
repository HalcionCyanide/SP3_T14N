#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "GameObject.h"
#include "Camera3.h"

class Boundary
{
public:
	Boundary();
	virtual ~Boundary();
	virtual bool CheckCollision(GameObject* object, Camera* player);
};








#endif