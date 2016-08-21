/****************************************************************************/
/*!
\file BaseObject.h
\author Ryan Lim Rui An
\par email: 150577L@mymail.nyp.edu.sg
\brief
Defines an object that exists within the 2D battle screen
*/
/****************************************************************************/

#ifndef _BASE_OBJECT_H
#define _BASE_OBJECT_H

#include "..\\Classes\\GenericEntity.h"
#include "..\\Classes\\Mesh.h"
#include "Vector3.h"
#include "Mtx44.h"
#include <string>

class BaseObject : public GenericEntity
{
public:
	// Constructors
	BaseObject();
	BaseObject(const std::string& MeshName, const float& Mass, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const float& RotationAngle, const Vector3& RotationAxis);

	// Destructor
	virtual ~BaseObject();

	bool Active;	 // Updates if true
	bool Visible;	 // Renders if it and Active is true 

	// Virtual
	virtual void Init(const std::string& MeshName, const float& Mass, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const float& RotationAngle, const Vector3& RotationAxis);
	virtual void Render();
	virtual void Update(double dt);
	virtual void Exit();

	// Setters
	void SetMesh(Mesh* &Mesh);
	void SetMesh(const std::string& MeshName);
	void SetPosition(const Vector3& Position);
	void SetDimensions(const Vector3& Dimensions);
	void SetRotationAngle(const float& RotationAngle);
	void SetRotationAxis(const Vector3& RotationAxis);
	void SetMass(const float& Mass);
	void SetVelocity(const Vector3& Velocity);
	void SetParameters(const std::string& MeshName, const float& Mass, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const float& RotationAngle, const Vector3& RotationAxis);

	// Getters
	Mesh* GetMesh() const;
	std::string GetMeshName() const;
	Vector3 GetPosition() const;
	Vector3 GetDimensions() const;
	float GetRotationAngle() const;
	Vector3 GetRotationAxis() const;
	float GetMass() const;
	Vector3 GetVelocity() const;

private:
	// Parameters
	Mesh* StoredMesh;
	std::string MeshName;
	Vector3 Position;
	Vector3 Dimensions;
	float RotationAngle;
	Vector3 RotationAxis;
	float Mass;
	Vector3 Velocity;

	// If other parameters are required, 
	// consider inheriting this class and adding them into the new class,
	// if the required parameters are not generic. ~ Ryan
};

#endif //_BASE_OBJECT_H