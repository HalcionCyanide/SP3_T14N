#include "GameObject.h"
#include "..\\Systems\\Scene_System.h"
#include "..\\Scenes\\GraphicsEntity.h"
#include "../Misc/LoadEnemyData.h"
#include "GateBoundary.h"

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
   // if (checkWhetherTheWordInThatString("gate", getName()))
   // {
   //     delete Bounds;
   //     std::string keyName =  getName();
   //     convertStringToUpperCaps(keyName);
   //     if (checkWhetherTheWordInThatString("TOWN1", keyName))
   //         Bounds = new GateBoundary(SceneTown1::id_);
   //     else if (checkWhetherTheWordInThatString("FREEFIELD", keyName))
			//Bounds = new GateBoundary(SceneFreeField::id_);
   //     else
   //         Bounds = new Boundary();
   // }
	Bounds->SetPosition(this->GetPosition());
	Bounds->SetDimensions(this->GetDimensions());
	Bounds->SetRotationAngle(this->GetRotationAngle());
	Bounds->ResetValues();
}

Boundary* GameObject::GetBoundary()const
{
	return Bounds;
}

GameObject& GameObject::operator= (const GameObject &object)
{
    name_ = object.name_;
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
        Bounds = nullptr;
	}
	Bounds = &theNewBounds;
    Bounds->SetPosition(this->GetPosition());
    Bounds->SetDimensions(this->GetDimensions());
    Bounds->SetRotationAngle(this->GetRotationAngle());
    Bounds->ResetValues();
}