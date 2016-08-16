#include "GameObject.h"
#include "Scene_System.h"
#include "GraphicsEntity.h"

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
	this->Rotation.SetToRotation(rotatingValue, rotationAxis.x, rotationAxis.y, rotationAxis.z);
}

void GameObject::SetActive(const bool &active)
{
	this->Active = active;
}

Mesh& GameObject::GetMesh()
{
	return *MeshObject;
}

Vector3 GameObject::GetPos()
{
	return this->Pos;
}

Vector3 GameObject::GetScale()
{
	return this->Scale;
}

Mtx44 GameObject::GetRotation()
{
	return this->Rotation;
}

bool GameObject::GetActive()
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
		Scene_System::accessing().getCurrScene().modelStack->Scale(Scale.x, Scale.y, Scale.z);
		SceneGraphics->RenderMesh(*MeshObject, false);
		Scene_System::accessing().getCurrScene().modelStack->PopMatrix();
	}
}