#include "Boundary.h"

Boundary::Boundary(const Vector3 &pos, const Vector3 &scale, const float &rotation)
{
	CalculateValues(pos, scale, rotation);
}

Boundary::Boundary(const Boundary &bounds)
{
	this->Position = bounds.Position;
	this->Scale = bounds.Scale;
	this->RotationValue = bounds.RotationValue;
}

bool Boundary::CheckCollision(const Boundary &object)const
{
	if (abs(this->Position.x - object.Position.x) > (this->Scale.x + object.Scale.x) * 0.5f) return false;
	if (abs(this->Position.z - object.Position.z) > (this->Scale.z + object.Scale.z) * 0.5f) return false;
	return true;
}

bool Boundary::CheckCollision(const Vector3 &point, const Vector3 &vel)const
{
	Vector3 w0 = this->Position;
	Vector3 b1 = point;
	Vector3 N = this->Normal;
	float h = this->Scale.x;
	float l = this->Scale.z;
	Vector3 NP = Vector3(-N.z, N.y ,N.x);
	Vector3 rV = vel;
	Vector3 rD = w0 - b1;

	if ((w0 - b1).Dot(N) < 0)
		N = -N;

	Vector3 fw0 = this->Position;
	Vector3 fb1 = point;
	Vector3 fN = this->FlipNormal;
	float fh = this->FlipScale.x;
	float fl = this->FlipScale.z;
	Vector3 fNP = Vector3(-fN.z, fN.y, fN.x);
	Vector3 frV = vel;
	Vector3 frD = fw0 - fb1;

	if ((fw0 - fb1).Dot(fN) < 0)
		fN = -fN;

	return (abs(rD.Dot(N)) < h / 2 && abs(rD.Dot(NP)) < l / 2 && rV.Dot(N) > 0)/* || (abs(frD.Dot(fN)) < fh / 2 && abs(frD.Dot(fNP)) < l / 2 && frV.Dot(fN) > 0)*/;
}

void Boundary::CalculateValues(const Vector3 &pos, const Vector3 &scale, const float &rotation)
{
	SetPosition(pos);
	SetScale(scale + Vector3(5, 5, 5));
	SetRotation(rotation);
	SetNormal();
}

void Boundary::SetPosition(const Vector3 &position)
{
	this->Position = position;
}

void Boundary::SetScale(const Vector3 &scale)
{
	this->Scale = scale;
	this->FlipScale = Vector3(scale.z, scale.y, scale.x);
}

void Boundary::SetRotation(const float &rotation)
{
	this->RotationValue = rotation;
}

void Boundary::SetNormal()
{
	Mtx44 Rotation;
	Rotation.SetToRotation(this->RotationValue, 0, 1, 0);
	this->Normal = Vector3(sin(Math::DegreeToRadian(RotationValue)), 0, cos(Math::DegreeToRadian(RotationValue)));
	Rotation.SetToZero();
	Rotation.SetToRotation(this->RotationValue + 180.f, 0, 1, 0);
	this->FlipNormal = Vector3(sin(Math::DegreeToRadian(RotationValue + 90.f)), 0, cos(Math::DegreeToRadian(RotationValue + 90.f)));
}

Vector3 Boundary::GetPosition()const
{
	return Position;
}

Vector3 Boundary::GetScale()const
{
	return Scale;
}

Vector3 Boundary::GetFlipScale()const
{
	return FlipScale;
}

float Boundary::GetRotation()const
{
	return RotationValue;
}

Vector3 Boundary::GetNormal()const
{
	return this->Normal;
}

Vector3 Boundary::GetFlipNormal()const
{
	return FlipNormal;
}