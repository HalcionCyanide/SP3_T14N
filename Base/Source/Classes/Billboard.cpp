#include "Billboard.h"
#include "..\\Systems\\Scene_System.h"
#include "..\\Scenes\\GraphicsEntity.h"

Billboard::Billboard()
    : GenericEntity()
{
    StoredMesh = nullptr;
    MeshName = "";
}

Billboard::Billboard(const Vector3& Position, const Vector3& Dimensions, const Vector3& PlayerPosition, const Vector3& Velocity, const std::string& MeshName, const float& LifeTime)
{
	Init(Position, Dimensions, PlayerPosition, Velocity, MeshName, LifeTime);
}

void Billboard::Init(const Vector3& Position, const Vector3& Dimensions, const Vector3& PlayerPosition, const Vector3& Velocity, const std::string& MeshName, const float& LifeTime)
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
    std::map<std::string, Mesh*>::iterator it = SceneGraphics->meshList.find(MeshName);
    setName(MeshName);
    if (it != SceneGraphics->meshList.end())
    {
        this->MeshName = MeshName;
        StoredMesh = it->second;
    }
    this->Position = Position;
    this->Dimensions = Dimensions;
    this->Velocity = Velocity;
    this->PlayerPosition = PlayerPosition;
    this->CurrentTime = 0;
    this->LifeTime = LifeTime;
}

void Billboard::Update(float dt)
{
	if (Active)
	{
		if (Active)
		{
			SetCurrentTime(GetCurrTime() + dt);
			SetPosition(GetPosition() + GetVelocity() * dt);
		}
		Active = CheckLife();
	}
}

void Billboard::Render()
{
	if (Active && StoredMesh)
	{
		float TimeRatio = 1;
		GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
		Scene_System::accessing().getCurrScene().modelStack->PushMatrix();
		Scene_System::accessing().getCurrScene().modelStack->Translate(Position.x, Position.y, Position.z); 
		if (LifeTime != -1)
		{
			TimeRatio = 1.1f - CurrentTime / LifeTime;
			Scene_System::accessing().getCurrScene().modelStack->Rotate(TimeRatio * 360, 0, 0, 1);
		}
		Scene_System::accessing().getCurrScene().modelStack->Scale(TimeRatio * Dimensions.x, TimeRatio * Dimensions.y, TimeRatio * Dimensions.z);
		if (LifeTime == -1)
			SceneGraphics->RenderMesh(*StoredMesh, true);
		else SceneGraphics->RenderMesh(*StoredMesh, false);
		Scene_System::accessing().getCurrScene().modelStack->PopMatrix();
	}
}

void Billboard::SetDefaultLifeTime()
{
	LifeTime = -1;
}

bool Billboard::CheckLife()
{
	if (LifeTime == -1)
		return true;
	return CurrentTime < LifeTime;
}

bool Billboard::operator<(Billboard& rhs){
	float V1 = (this->Position - this->PlayerPosition).LengthSquared();
	float V2 = (rhs.Position - this->PlayerPosition).LengthSquared();
	return (V1 > V2);
}

Vector3 Billboard::GetPosition()
{
	return Position;
}

Vector3 Billboard::GetDimensions()
{
	return Dimensions;
}

Vector3 Billboard::GetVelocity()
{
	return Velocity;
}

std::string Billboard::GetMeshName()
{
	return MeshName;
}

float Billboard::GetCurrTime()
{
	return CurrentTime;
}

float Billboard::GetLifeTime()
{
	return LifeTime;
}

void Billboard::SetCurrentTime(float t)
{
	CurrentTime = t;
}

void Billboard::SetPosition(const Vector3& v3)
{
	Position = v3;
}

void Billboard::SetPlayerPosition(const Vector3& v3)
{
	PlayerPosition = v3;
}

void Billboard::SetParameters(const Vector3& Position, const Vector3& Dimensions, const Vector3& PlayerPosition, const Vector3& Velocity, const std::string& MeshName, const float& CurrentTime, const float& LifeTime)
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	std::map<std::string, Mesh*>::iterator it = SceneGraphics->meshList.find(MeshName);
	if (it != SceneGraphics->meshList.end())
	{
		this->MeshName = MeshName;
		this->Position = Position;
		this->Dimensions = Dimensions;
		this->Velocity = Velocity;
		this->PlayerPosition = PlayerPosition;
		this->CurrentTime = 0;
		this->LifeTime = LifeTime;
		StoredMesh = it->second;
	}
}