#include "GameObject.h"
#include "Scene_System.h"
#include "GraphicsEntity.h"

GameObject::GameObject(const std::string &name, const Vector3 &pos, const Vector3 &scale, const float &rotatingValue, const Vector3 &rotationAxis, const bool &active)
{
	SetPos(pos);
	SetScale(scale);
	SetActive(active);
	setName(name);
	SetRotation(rotatingValue, rotationAxis);
	SetBounds();
};

GameObject::GameObject(const GameObject &object)
{
	this->MeshObject = object.MeshObject;
	this->Pos = object.Pos;
	this->Scale = object.Scale;
	this->RotationAngle = object.RotationAngle;
	this->Active = object.Active;
	SetBounds();
}

void GameObject::SetMesh(Mesh &meshObject)
{
	this->MeshObject = &meshObject;
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
	Bounds.CalculateValues(this->Pos, this->Scale);
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
		SceneGraphics->RenderMesh(*MeshObject, false);
		Scene_System::accessing().getCurrScene().modelStack->PopMatrix();
	}
}

GameObject& GameObject::operator= (GameObject* object)
{
	if (this != object)
	{
		this->MeshObject = object->MeshObject;
		this->Pos = object->Pos;
		this->Scale = object->Scale;
		this->RotationAngle = object->RotationAngle;
		this->Active = object->Active;
	}
}