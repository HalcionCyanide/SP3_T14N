#include "GameObject.h"
#include "..\\Systems\\Scene_System.h"
#include "..\\Scenes\\GraphicsEntity.h"

GameObject::GameObject()
	//: BaseObject()
{
	Bounds = nullptr;
	SetMesh(nullptr);
}

GameObject::GameObject(const GameObject& rhs)
{
	Bounds = nullptr;
	*this = rhs;
}

GameObject::~GameObject() 
{
	Exit();
}

void GameObject::Exit()
{
	if (Bounds)
	{
		delete Bounds;
		Bounds = nullptr;
	}
}

void GameObject::SetBounds()
{
	if (Bounds == nullptr)
		Bounds = new Boundary();
	Bounds->CalculateValues(GetPosition(), GetDimensions(), GetRotationAngle());
}

Boundary* GameObject::GetBoundary()const
{
	return Bounds;
}

GameObject& GameObject::operator= (const GameObject &object)
{
	SetMesh(object.GetMesh());
	SetPosition(object.GetPosition());
	SetDimensions(object.GetDimensions());
	SetVelocity(object.GetVelocity());
	SetRotationAngle(object.GetRotationAngle());
	SetRotationAxis(object.GetRotationAxis());
	SetMass(object.GetMass());
	SetBounds();
	return *this;
}

void GameObject::settingNewBounds(Boundary &theNewBounds)
{
	if (Bounds)
	{
		delete Bounds;
	}
	Bounds = &theNewBounds;
}