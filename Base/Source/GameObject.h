#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "GenericEntity.h"
#include "Vector3.h"
#include "Mtx44.h"
#include "Mesh.h"
#include <string>

class GameObject : public GenericEntity
{
public:
	GameObject(){};
	GameObject(const std::string &name, const Vector3 &pos, const Vector3 &scale, const float &rotatingValue, const Vector3 &rotationAxis, const bool &active) :
		Pos(pos), Scale(scale), Active(active) {
		setName(name);
		SetRotation(rotatingValue, rotationAxis);
	};
	virtual ~GameObject(){};

	virtual void Init(const std::string &name, const Vector3 &pos, const Vector3 &scale, const float &rotatingValue, const Vector3 &rotationAxis, const bool &active);
	virtual void Render();

	virtual void SetMesh(Mesh &meshObject);
	virtual void SetPos(const Vector3 &pos);
	virtual void SetScale(const Vector3 &scale);
	virtual void SetRotation(const float &rotatingValue, const Vector3 &rotationAxis);
	virtual void SetActive(const bool &active);

	Mesh& GetMesh();
	Vector3 GetPos();
	Vector3 GetScale();
	Mtx44 GetRotation();
	bool GetActive();

private:

	Mesh* MeshObject = nullptr;
	Vector3 Pos, Scale;
	Mtx44 Rotation;
	bool Active = true;

};

#endif