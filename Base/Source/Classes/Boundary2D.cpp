#include "Boundary2D.h"

Boundary2D::Boundary2D() :Boundary()
{}

Boundary2D::Boundary2D(const Vector3 &pos, const Vector3 &scale, const float &rotationAngle)
{
	Vertices = nullptr;
	Axes = nullptr;
	CalculateValues(pos, scale, rotationAngle);
}

Boundary2D::Boundary2D(const Boundary2D &bounds)
{
	this->SetPosition(bounds.GetPosition());
	this->SetScale(bounds.GetScale());
	this->Vertices = bounds.Vertices;
	this->Axes = bounds.Axes;
}

Boundary2D::~Boundary2D()
{
}

bool Boundary2D::CheckCollision(const Boundary2D &object)const
{
	if (abs(this->GetPosition().x - object.GetPosition().x) > (this->GetScale().x + object.GetScale().x) * 0.5f) return false;
	if (abs(this->GetPosition().y - object.GetPosition().y) > (this->GetScale().y + object.GetScale().y) * 0.5f) return false;
	return true;
}

bool Boundary2D::CheckCollision(const Vector3 &point)const
{
	if (abs(this->GetPosition().x - point.x) > (this->GetScale().x) * 0.5f) return false;
	if (abs(this->GetPosition().y - point.y) > (this->GetScale().y) * 0.5f) return false;
	return true;
}