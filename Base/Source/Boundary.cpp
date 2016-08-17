#include "Boundary.h"

Boundary::Boundary(const Vector3 &pos, const Vector3 &scale)
{
	CalculateValues(pos, scale);
}

Boundary::Boundary(const Boundary &bounds)
{
	this->Position = bounds.Position;
	this->Scale = bounds.Scale;
}

bool Boundary::CheckCollision(const Boundary &object)const
{
	if (abs(this->Position.x - object.Position.x) > (this->Scale.x + object.Scale.x) * 0.5f) return false;
//	if (abs(this->Position.y - object.Position.y) > (this->Scale.y + object.Scale.y) * 0.5f) return false;
	if (abs(this->Position.z - object.Position.z) > (this->Scale.z + object.Scale.z) * 0.5f) return false;
	return true;
}

bool Boundary::CheckCollision(const Vector3 &point)const
{
	if (abs(this->Position.x - point.x) > (this->Scale.x) * 0.5f) return false;
	if (abs(this->Position.z - point.z) > (this->Scale.z) * 0.5f) return false;
	return true;
}

void Boundary::CalculateValues(const Vector3 &pos, const Vector3 &scale)
{
	SetPosition(pos);
	SetScale(scale);
}

void Boundary::SetPosition(const Vector3 &position)
{
	this->Position = position;
}

void Boundary::SetScale(const Vector3 &scale)
{
	this->Scale = scale;
}

Vector3 Boundary::GetPosition()const
{
	return Position;
}

Vector3 Boundary::GetScale()const
{
	return Scale;
}