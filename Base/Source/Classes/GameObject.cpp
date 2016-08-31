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
    if (checkWhetherTheWordInThatString("Scene", getName()))
    {
        delete Bounds;
		Bounds = new GateBoundary();
		Bounds->setName(getName());
		if (Bounds->getName() == "2_Scene")
		{
			GateBoundary* TempBound = dynamic_cast<GateBoundary*>(Bounds);
			TempBound->SetQuestData("The Fractal Binding", 1);
		}
		if (Bounds->getName() == "3_Scene")
		{
			GateBoundary* TempBound = dynamic_cast<GateBoundary*>(Bounds);
			TempBound->SetQuestData("The Fractal Binding", 5);
		}
    }
	else if (checkWhetherTheWordInThatString("Boss", getName()))
	{
		delete Bounds;

	}

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