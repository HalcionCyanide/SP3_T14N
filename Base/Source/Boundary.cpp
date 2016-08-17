#include "Boundary.h"

Boundary::Boundary(const Vector3 &pos, const Vector3 &scale)
{
	CalculateValues(pos, scale);
}

Boundary::Boundary(const Boundary &bounds)
{
	if (this != &bounds)
	{
		this->MaxValues = bounds.MaxValues;
		this->MinValues = bounds.MinValues;
	}
}

bool Boundary::CheckCollision(const Boundary &object, const Boundary &player)const
{
	return ((object.GetMaxValues().x > player.GetMinValues().x || player.GetMaxValues().x > object.GetMinValues().x) &&
		(object.GetMaxValues().y > player.GetMinValues().y || player.GetMaxValues().y > object.GetMinValues().y) &&
		(object.GetMaxValues().z > player.GetMinValues().z || player.GetMaxValues().z > object.GetMinValues().z));
}

bool Boundary::CheckCollision2D(const Boundary &object, const Vector3 &point)const
{
	return ((object.GetMaxValues().x > point.x || point.x > object.GetMinValues.x) &&
		(object.GetMaxValues().y > point.y || point.y > object.GetMinValues().y));
}

void Boundary::CalculateValues(const Vector3 &pos, const Vector3 &scale)
{
	Vector3 ObjectMin, ObjectMax;
	ObjectMax.Set(float(pos.x + (scale.x * 0.5f)), float(pos.y + (scale.y * 0.5f)), float(pos.z + (scale.z * 0.5f)));
	ObjectMin.Set(float(pos.x - (scale.x * 0.5f)), float(pos.y - (scale.y * 0.5f)), float(pos.z - (scale.z * 0.5f)));

	SetMaxValues(ObjectMax);
	SetMinValues(ObjectMin);
}

void Boundary::SetMaxValues(const Vector3 &maxvalues)
{
	this->MaxValues = maxvalues;
}

void Boundary::SetMinValues(const Vector3 &minValues)
{
	this->MinValues = minValues;
}

Vector3 Boundary::GetMaxValues()const
{
	return MaxValues;
}

Vector3 Boundary::GetMinValues()const
{
	return MinValues;
}