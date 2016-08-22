#include "Boundary2D.h"

Boundary2D::Boundary2D() :Boundary()
{}

Boundary2D::Boundary2D(const Vector3 &pos, const Vector3 &scale, const float &rotationAngle)
{
	Vertices = nullptr;
	Axis = nullptr;
	SetVerticeNo(4);
	ResetValues(pos, scale, rotationAngle);
}

Boundary2D::Boundary2D(const Boundary2D &bounds)
{
	this->SetPosition(bounds.GetPosition());
	this->SetDimension(bounds.GetDimension());
	this->SetVerticeNo(bounds.GetVerticeNo());
	this->Vertices = bounds.Vertices;
	this->Axis = bounds.Axis;
}

Boundary2D::~Boundary2D()
{
}

bool Boundary2D::CheckCollision(const Boundary2D &object)const
{
	if (abs(this->GetPosition().x - object.GetPosition().x) > (this->GetDimension().x + object.GetDimension().x) * 0.5f) return false;
	if (abs(this->GetPosition().y - object.GetPosition().y) > (this->GetDimension().y + object.GetDimension().y) * 0.5f) return false;
	return true;
}

bool Boundary2D::CheckCollision(const Vector3 &point)const
{
	if (abs(this->GetPosition().x - point.x) > (this->GetDimension().x) * 0.5f) return false;
	if (abs(this->GetPosition().y - point.y) > (this->GetDimension().y) * 0.5f) return false;
	return true;
}