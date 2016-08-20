/****************************************************************************/
/*!
\file BattleScreenObject.cpp
\author Ryan Lim Rui An
\par email: 150577L@mymail.nyp.edu.sg
\brief
Defines an object that exists within the 2D battle screen
*/
/****************************************************************************/

#include "BattleScreenObject.h"
#include "..\\Systems\\Scene_System.h"
#include "..\\Scenes\\GraphicsEntity.h"

// Constructors
BattleScreenObject::BattleScreenObject()
{
	SetParameters("", 0, Vector3(), Vector3(1, 1, 1), Vector3(), 0, Vector3(0, 0, 1));
}

BattleScreenObject::BattleScreenObject(const std::string& MeshName, const float& Mass, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const float& RotationAngle, const Vector3& RotationAxis)
{
	Init(MeshName, Mass, Position, Dimensions, Velocity, RotationAngle, RotationAxis);
}

// Destructor
BattleScreenObject::~BattleScreenObject()
{
	Exit();
}

// Virtual
void BattleScreenObject::Init(const std::string& MeshName, const float& Mass, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const float& RotationAngle, const Vector3& RotationAxis)
{
	Active = true;
	SetParameters(MeshName, Mass, Position, Dimensions, Velocity, RotationAngle, RotationAxis);
}

void BattleScreenObject::Render()
{
	if (Active)
	{
		GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
		Scene_System::accessing().getCurrScene().modelStack->PushMatrix();
		Scene_System::accessing().getCurrScene().modelStack->Translate(Position.x, Position.y, Position.z);
		Scene_System::accessing().getCurrScene().modelStack->Rotate(RotationAngle, RotationAxis.x, RotationAxis.y, RotationAxis.z);
		Scene_System::accessing().getCurrScene().modelStack->Scale(Dimensions.x, Dimensions.y, Dimensions.z);
		SceneGraphics->RenderMesh(*StoredMesh, false);
		Scene_System::accessing().getCurrScene().modelStack->PopMatrix();
	}
}

void BattleScreenObject::Update(double dt)
{
	Vector3 StoredVelocity = Velocity;
	//Velocityl += m_gravity * dt; // For Gravity
	Position += (StoredVelocity + Velocity) * 0.5f * (float)dt;
	RotationAngle = Math::RadianToDegree(atan2(-Velocity.x, Velocity.y));
}

void BattleScreenObject::Exit()
{
	// Clean up if necessary
}

// Setters
void BattleScreenObject::SetMesh(Mesh* &Mesh)
{
	this->StoredMesh = Mesh;
}

void BattleScreenObject::SetMesh(const std::string& MeshName)
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	std::map<std::string, Mesh*>::iterator it = SceneGraphics->meshList.find(MeshName);
	if (it != SceneGraphics->meshList.end())
	{
		this->MeshName = MeshName;
		this->StoredMesh = it->second;
	}
}

void BattleScreenObject::SetPosition(const Vector3& Position)
{
	this->Position = Position;
}

void BattleScreenObject::SetDimensions(const Vector3& Dimensions)
{
	this->Dimensions = Dimensions;
}

void BattleScreenObject::SetRotationAngle(const float& RotationAngle)
{
	this->RotationAngle = RotationAngle;
}

void BattleScreenObject::SetRotationAxis(const Vector3& RotationAxis)
{
	this->RotationAxis = RotationAxis;
}

void BattleScreenObject::SetMass(const float& Mass)
{
	this->Mass = Mass;
}

void BattleScreenObject::SetParameters(const std::string& MeshName, const float& Mass, const Vector3& Position, const Vector3& Dimensions, const Vector3& Velocity, const float& RotationAngle, const Vector3& RotationAxis)
{
	SetMesh(MeshName);
	this->Position = Position;
	this->Dimensions = Dimensions;
	this->Velocity = Velocity;
	this->RotationAngle = RotationAngle;
	this->RotationAxis = RotationAxis;
	this->Mass = Mass;
}

void  BattleScreenObject::SetVelocity(const Vector3& Velocity)
{
	this->Velocity = Velocity;
}

// Getters
Mesh* BattleScreenObject::GetMesh() const
{
	return StoredMesh;
}

std::string BattleScreenObject::GetMeshName() const
{
	return MeshName;
}

Vector3 BattleScreenObject::GetPosition() const
{
	return Position;
}

Vector3 BattleScreenObject::GetDimensions() const
{
	return Dimensions;
}

float BattleScreenObject::GetRotationAngle() const
{
	return RotationAngle;
}

Vector3 BattleScreenObject::GetRotationAxis() const
{
	return RotationAxis;
}

float BattleScreenObject::GetMass() const
{
	return Mass;
}

Vector3 BattleScreenObject::GetVelocity() const
{
	return Velocity;
}