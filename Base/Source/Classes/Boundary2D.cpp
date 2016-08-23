#include "Boundary2D.h"

Boundary2D::Boundary2D()
{}

Boundary2D::Boundary2D(const Vector3 &pos, const Vector3 &scale, const float &rotationAngle)
{
	SetVerticeNo(4);
}

Boundary2D::Boundary2D(const Boundary2D &bounds)
{
	//this->SetPosition(bounds.GetPosition());
	//this->SetDimension(bounds.GetDimension());
}

Boundary2D::~Boundary2D()
{
}

bool Boundary2D::CheckCollision(const Boundary2D &object)const
{
	if (abs(this->GetPosition().x - object.GetPosition().x) > (this->GetDimensions().x + object.GetDimensions().x) * 0.5f) return false;
	if (abs(this->GetPosition().y - object.GetPosition().y) > (this->GetDimensions().y + object.GetDimensions().y) * 0.5f) return false;
	return true;
}

bool Boundary2D::CheckCollision(const Vector3 &point)const
{
	if (abs(this->GetPosition().x - point.x) > (this->GetDimensions().x) * 0.5f) return false;
	if (abs(this->GetPosition().y - point.y) > (this->GetDimensions().y) * 0.5f) return false;
	return true;
}