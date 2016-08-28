/****************************************************************************/
/*!
\file BaseObject.cpp
\author Ryan Lim Rui An
\par email: 150577L@mymail.nyp.edu.sg
\brief
Defines the minimum standard for an object that exists within the game
*/
/****************************************************************************/

#include "BaseObject.h"
#include "..\\Systems\\Scene_System.h"
#include "..\\Scenes\\GraphicsEntity.h"

// Constructors
BaseObject::BaseObject()
{
	SetParameters(nullptr, 0, Vector3(), Vector3(1, 1, 1), Vector3(), 0, Vector3(0, 0, 1));
}

BaseObject::BaseObject(const std::string& MeshName, const float& Mass, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const float& RotationAngle, const Vector3& RotationAxis)
{
	Init(MeshName, Mass, Position, Dimensions, Velocity, RotationAngle, RotationAxis);
}

// Destructor
BaseObject::~BaseObject()
{
	Exit();
}

// Virtual
void BaseObject::Init(const std::string& MeshName, const float& Mass, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const float& RotationAngle, const Vector3& RotationAxis)
{
	Active = true;
	Visible = true;
	SetParameters(MeshName, Mass, Position, Dimensions, Velocity, RotationAngle, RotationAxis);
}

void BaseObject::Render()
{
	if (Active && Visible)
	{
		GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
		Scene_System::accessing().getCurrScene().modelStack->PushMatrix();
		Scene_System::accessing().getCurrScene().modelStack->Translate(Position.x, Position.y, Position.z);
		Scene_System::accessing().getCurrScene().modelStack->Rotate(RotationAngle, RotationAxis.x, RotationAxis.y, RotationAxis.z);
		Scene_System::accessing().getCurrScene().modelStack->Scale(Dimensions.x, Dimensions.y, Dimensions.z);
		SceneGraphics->RenderMesh(*StoredMesh, true);
		Scene_System::accessing().getCurrScene().modelStack->PopMatrix();
	}
}

void BaseObject::Update(double dt)
{
	if (Active) // Still can update if invisible
	{
		Vector3 StoredVelocity = Velocity;
		//Velocityl += m_gravity * dt; // For Gravity
		Position += (StoredVelocity + Velocity) * 0.5f * (float)dt;
		RotationAngle = Math::RadianToDegree(atan2(-Velocity.x, Velocity.y));
	}
}

void BaseObject::Exit()
{
	// Clean up if necessary
}

// Setters
void BaseObject::SetMesh(Mesh* Mesh)
{
	this->StoredMesh = Mesh;
}

void BaseObject::SetMesh(const std::string& MeshName)
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	std::map<std::string, Mesh*>::iterator it = SceneGraphics->meshList.find(MeshName);
	if (it != SceneGraphics->meshList.end())
	{
		this->MeshName = MeshName;
		this->StoredMesh = it->second;
	}
}

void BaseObject::SetPosition(const Vector3& Position)
{
	this->Position = Position;
}

void BaseObject::SetDimensions(const Vector3& Dimensions)
{
	this->Dimensions = Dimensions;
}

void BaseObject::SetRotationAngle(const float& RotationAngle)
{
	this->RotationAngle = RotationAngle;
}

void BaseObject::SetRotationAxis(const Vector3& RotationAxis)
{
	this->RotationAxis = RotationAxis;
}

void BaseObject::SetMass(const float& Mass)
{
	this->Mass = Mass;
}

void BaseObject::SetParameters(const std::string& MeshName, const float& Mass, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const float& RotationAngle, const Vector3& RotationAxis)
{
	SetMesh(MeshName);
	this->Position = Position;
	this->Dimensions = Dimensions;
	this->Velocity = Velocity;
	this->RotationAngle = RotationAngle;
	this->RotationAxis = RotationAxis;
	this->Mass = Mass;
}

void BaseObject::SetParameters(Mesh* Mesh, const float& Mass, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const float& RotationAngle, const Vector3& RotationAxis)
{
	SetMesh(Mesh);
	this->Position = Position;
	this->Dimensions = Dimensions;
	this->Velocity = Velocity;
	this->RotationAngle = RotationAngle;
	this->RotationAxis = RotationAxis;
	this->Mass = Mass;
}

void  BaseObject::SetVelocity(const Vector3& Velocity)
{
	this->Velocity = Velocity;
}

// Getters
Mesh* BaseObject::GetMesh() const
{
	return StoredMesh;
}

std::string BaseObject::GetMeshName() const
{
	return MeshName;
}

Vector3 BaseObject::GetPosition() const
{
	return Position;
}

Vector3 BaseObject::GetDimensions() const
{
	return Dimensions;
}

float BaseObject::GetRotationAngle() const
{
	return RotationAngle;
}

Vector3 BaseObject::GetRotationAxis() const
{
	return RotationAxis;
}

float BaseObject::GetMass() const
{
	return Mass;
}

Vector3 BaseObject::GetVelocity() const
{
	return Velocity;
}