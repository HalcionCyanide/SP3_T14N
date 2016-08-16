#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "GraphicsEntity.h"

class GameObject : public GenericEntity
{
public:
	GameObject();
	~GameObject();

	void SetObject(Mesh* object);

	void SetPos(Vector3 pos);
	void SetScale(Vector3 scale);
	void SetRotation(float rotatingValue, Vector3 rotationAxis);

	Vector3 GetPos(void);
	Vector3 GetScale(void);
	Mtx44 GetRotation(void);

private:
	Mesh* Object;
	Vector3 Pos, Scale;
	Mtx44 Rotation;
};



#endif