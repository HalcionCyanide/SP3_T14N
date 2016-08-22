#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "..//Classes/BaseObject.h"
#include "Vector3.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "Boundary.h"
#include <string>

class GameObject : public BaseObject
{
public:
	GameObject();
	GameObject(const GameObject&);
	
	virtual ~GameObject();

	virtual void Exit();

	virtual void SetBounds();

	virtual void settingNewBounds(Boundary &theNewBounds);

	Boundary* GetBoundary()const;

	GameObject& operator= (const GameObject& object);

protected:
	Boundary* Bounds;
};

#endif