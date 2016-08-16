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

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::SetObject(Mesh* object)
{
	this->Object = object;
}

void GameObject::SetPos(Vector3 pos)
{
	this->Pos = pos;
}

void GameObject::SetScale(Vector3 scale)
{
	this->Scale = scale;
}

void GameObject::SetRotation(float rotatingValue, Vector3 rotationAxis)
{
	this->Rotation.SetToRotation(rotatingValue, rotationAxis.x, rotationAxis.y, rotationAxis.z);
}

Vector3 GameObject::GetPos(void)
{
	return this->Pos;
}

Vector3 GameObject::GetScale(void)
{
	return this->Scale;
}

Mtx44 GameObject::GetRotation(void)
{
	return this->Rotation;
}




#endif