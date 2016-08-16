#include "Billboard.h"

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

void Billboard::SetParameters(Vector3 Position, Vector3 Dimensions, Vector3 PlayerPosition, Vector3 Velocity, std::string MeshName, float CurrentTime, float LifeTime)
{
	this->MeshName = MeshName;
	this->Position = Position;
	this->Dimensions = Dimensions;
	this->Velocity = Velocity;
	this->PlayerPosition = PlayerPosition;
	this->CurrentTime = 0;
	this->LifeTime = LifeTime;
}