#include "GameObject.h"
#include "..\\Systems\\Scene_System.h"
#include "..\\Scenes\\GraphicsEntity.h"

GameObject::GameObject()
	: GenericEntity()
{
	RotationAngle = 0;
	Bounds = nullptr;
}

GameObject::GameObject(const std::string &name, const Vector3 &pos, const Vector3 &scale, const float &rotatingValue, const Vector3 &rotationAxis, const bool &active) :
Pos(pos), Scale(scale), Active(active)
{
	SetMesh(name);
	setName(name);
	SetRotation(rotatingValue, rotationAxis);
	Bounds = nullptr;
}

GameObject::GameObject(GameObject &Object)
{
	this->MeshObject = Object.MeshObject;
	this->Pos = Object.Pos;
	this->Scale = Object.Scale;
	this->RotationAngle = Object.RotationAngle;
	this->Active = Object.Active;
	setName(Object.getName());
	if (Object.Bounds)
	{
		Bounds = new Boundary();
		Bounds->CalculateValues(this->Pos, this->Scale, this->RotationAngle);
	}
}

GameObject::~GameObject()
{
	if (Bounds)
	{
		delete Bounds;
		Bounds = nullptr;
	}
}

void GameObject::SetMesh(Mesh &meshObject)
{
	this->MeshObject = &meshObject;
}

void GameObject::SetMesh(const std::string& name)
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	std::map<std::string, Mesh*>::iterator it = SceneGraphics->meshList.find(name);
	if (it != SceneGraphics->meshList.end())
		this->MeshObject = it->second;
}

void GameObject::SetPos(const Vector3 &pos)
{
	this->Pos = pos;
}

void GameObject::SetScale(const Vector3 &scale)
{
	this->Scale = scale;
}

void GameObject::SetRotation(const float &rotatingValue, const Vector3 &rotationAxis)
{
	this->RotationAngle = rotatingValue;
}

void GameObject::SetActive(const bool &active)
{
	this->Active = active;
}

void GameObject::SetBounds()
{
	if (Bounds == nullptr)
		Bounds = new Boundary();
	Bounds->CalculateValues(this->Pos, this->Scale, this->RotationAngle);
}

Mesh& GameObject::GetMesh()const
{
	return *MeshObject;
}

Vector3 GameObject::GetPos()const
{
	return this->Pos;
}

Vector3 GameObject::GetScale()const
{
	return this->Scale;
}

float GameObject::GetRotation()const
{
	return this->RotationAngle;
}

bool GameObject::GetActive()const
{
	return this->Active;
}

Boundary* GameObject::GetBoundary()const
{
	return Bounds;
}

void GameObject::Init(const std::string &name, const Vector3 &pos, const Vector3 &scale, const float &rotatingValue, const Vector3 &rotationAxis, const bool &active)
{
	this->setName(name);
	this->SetPos(pos);
	this->SetScale(scale);
	this->SetRotation(rotatingValue, rotationAxis);
	this->SetActive(active);
}

void GameObject::Render()
{
	if (Active && MeshObject)
	{
		GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
		Scene_System::accessing().getCurrScene().modelStack->PushMatrix();
		Scene_System::accessing().getCurrScene().modelStack->Translate(Pos.x, Pos.y, Pos.z);
		Scene_System::accessing().getCurrScene().modelStack->Rotate(RotationAngle, 0, 1, 0);
		Scene_System::accessing().getCurrScene().modelStack->Scale(Scale.x, Scale.y, Scale.z);
		SceneGraphics->RenderMesh(*MeshObject, true);
		Scene_System::accessing().getCurrScene().modelStack->PopMatrix();
	}
}

GameObject& GameObject::operator= (const GameObject &object)
{
	this->MeshObject = object.MeshObject;
	this->Pos = object.Pos;
	this->Scale = object.Scale;
	this->RotationAngle = object.RotationAngle;
	this->Active = object.Active;
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