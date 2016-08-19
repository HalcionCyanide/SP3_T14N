#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "GenericEntity.h"
#include "Vector3.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "Boundary.h"
#include <string>

class GameObject : public GenericEntity
{
public:
	GameObject();
    GameObject(const std::string &name, const Vector3 &pos, const Vector3 &scale, const float &rotatingValue, const Vector3 &rotationAxis, const bool &active);
    GameObject(GameObject &Object);

	virtual ~GameObject();

	virtual void Init(const std::string &name, const Vector3 &pos, const Vector3 &scale, const float &rotatingValue, const Vector3 &rotationAxis, const bool &active);
	virtual void Render();

	virtual void SetMesh(Mesh &meshObject);
	virtual void SetMesh(const std::string& name);
	virtual void SetPos(const Vector3 &pos);
	virtual void SetScale(const Vector3 &scale);
	virtual void SetRotation(const float &rotatingValue, const Vector3 &rotationAxis);
	virtual void SetActive(const bool &active);
	virtual void SetBounds();

    virtual void settingNewBounds(Boundary &theNewBounds);

	Mesh& GetMesh()const;
	Vector3 GetPos()const;
	Vector3 GetScale()const;
	float GetRotation()const;
	bool GetActive()const;
	Boundary* GetBoundary()const;

	GameObject& operator= (const GameObject& object);

protected:

	Mesh* MeshObject = nullptr;
	Vector3 Pos, Scale;
	float RotationAngle;
	bool Active = true;
	Boundary* Bounds;
};

#endif