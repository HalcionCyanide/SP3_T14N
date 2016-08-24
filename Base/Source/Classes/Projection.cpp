#include "Projection.h"


Projection::Projection(const float &min, const float &max)
{
	this->min = min;
	this->max = max;
}

Projection::Projection(const Projection &proj)
{
	this->min = proj.min;
	this->max = proj.max;
}

Projection::~Projection()
{
}

void Projection::SetMin(const float &min)
{
	this->min = min;
}

void Projection::SetMax(const float &max)
{
	this->max = max;
}

float Projection::GetMin()const
{
	return this->min;
}

float Projection::GetMax()const
{
	return this->max;
}

bool Projection::DetermineCollision(const Projection &value)
{
	return ((this->min < value.max && this->max > value.max) || (this->max > value.min && this->min < value.min));
}

bool Projection::DetermineCollision(const float &value)
{
	return (value < this->min && value < this->max);
}

float Projection::GetOverlappingDistance(const Projection &value)const
{
	if (this->min < value.max && this->max > value.max)
		return value.max - this->min;
	else if (this->max > value.min && this->min < value.min)
		return this->max - value.min;
	return 0.f;
}
