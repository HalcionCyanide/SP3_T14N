#ifndef _UI_ELEMENT_H
#define _UI_ELEMENT_H

#include "GenericEntity.h"
#include "Vector3.h"
#include "Boundary.h"

class Boundary;

class UI_Element : public GenericEntity
{
public:
	UI_Element();
	virtual ~UI_Element();

	Vector3 Position;
	Vector3 Dimensions;
	Vector3 Velocity;

	Boundary UI_Bounds;

	virtual void Init() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;

private:

};



#endif //_UI_ELEMENT_H