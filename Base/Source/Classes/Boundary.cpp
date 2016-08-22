#include "Boundary.h"

Boundary::Boundary()
{
	Vertices = nullptr;
	Axes = nullptr;
}

Boundary::Boundary(const Vector3 &pos, const Vector3 &scale, const float &rotation)
{
	Vertices = nullptr;
	Axes = nullptr;
	CalculateValues(pos, scale, rotation);
}

Boundary::Boundary(const Boundary &bounds)
{
	this->Position = bounds.Position;
	this->Scale = bounds.Scale;
	this->RotationValue = bounds.RotationValue;
	this->Axes = bounds.Axes;
	this->Vertices = bounds.Vertices;
	this->VerticesNo = bounds.VerticesNo;
}

Boundary::~Boundary()
{
	if (Axes)
	{
		delete []Axes;
		Axes = nullptr;
	}
	if (Vertices)
	{
		delete[]Vertices;
		Vertices = nullptr;
	}
}

bool Boundary::CheckCollision(Boundary &object)
{
	for (int i = 0; i < VerticesNo; ++i)
	{
		Projection* point1 = &Projecting(this->Axes[i]);
		Projection* point2 = &object.Projecting(this->Axes[i]);
		if (!point1->DetermineCollision(*point2))
			return false;
	}
	for (int i = 0; i < VerticesNo; ++i)
	{
		Projection* point1 = &Projecting(object.Axes[i]);
		Projection* point2 = &object.Projecting(object.Axes[i]);
		if (!point1->DetermineCollision(*point2))
			return false;
	}
	return true;
}

bool Boundary::CheckCollision(const Vector3 &point)
{
	for (int i = 0; i < VerticesNo; ++i)
	{
		Projection* point1 = &Projecting(this->Axes[i]);
		Projection* point2 = &ProjectingPoint(point, this->Axes[i]);
		if (point1->DetermineCollision(*point2))
			return false;
	}
	return true;
}

void Boundary::CalculateValues(const Vector3 &pos, const Vector3 &scale, const float &rotation)
{
	SetPosition(pos);
	SetScale(scale + Vector3(5, 5, 5));
	SetRotation(rotation);
	SetVertices();
	SetAxes();
}

void Boundary::SetPosition(const Vector3 &position)
{
	this->Position = position;
}

void Boundary::SetScale(const Vector3 &scale)
{
	this->Scale = scale;
}

void Boundary::SetRotation(const float &rotation)
{
	this->RotationValue = rotation;
}

Vector3 Boundary::GetPosition()const
{
	return Position;
}

Vector3 Boundary::GetScale()const
{
	return Scale;
}

void Boundary::SetAxes()
{
	if (Axes)
	{
		delete[]Axes;
		Axes = nullptr;
	}
	this->Axes = new Vector3[VerticesNo];
	for (int i = 0; i < VerticesNo; ++i)
	{
		Vector3 point1 = Vertices[i];
		Vector3 point2 = Vertices[i + 1 == VerticesNo ? 0 : i + 1];
		Vector3 N = point1.Cross(point2);
		Axes[i] = N;
		Axes[i].Normalize();
	}
}

void Boundary::SetVertices()
{
	//Currently limit to 4 Vertices only, will need to change if have more
	if (Vertices)
	{
		delete[]Vertices;
		Vertices = nullptr;
	}
	Vertices = new Vector3[VerticesNo];
	Mtx44 Rotation;
	Rotation.SetToRotation(this->RotationValue, 0, 1, 0);
	Vector3 Scale = Rotation * this->Scale;
	Vertices[0] = Vector3(this->Position.x + Scale.x/2, 0, this->Position.z + Scale.z/2);
	Vertices[1] = Vector3(this->Position.x + Scale.x/2, 0, this->Position.z - Scale.z/2);
	Vertices[2] = Vector3(this->Position.x - Scale.x/2, 0, this->Position.z - Scale.z/2);
	Vertices[3] = Vector3(this->Position.x - Scale.x/2, 0, this->Position.z + Scale.z/2);
}

void Boundary::SetVerticeNo(const int &value)
{
	this->VerticesNo = value;
}

Projection& Boundary::Projecting(const Vector3 &axis)
{
	float min, max;
	min = max = axis.Dot(this->Vertices[0]);
	for (int i = 1; i < VerticesNo; ++i)
	{
		float value = axis.Dot(this->Vertices[i]);
		if (value < min)
			min = value;
		else if (value > max)
			max = value;
	}
	Projection proj(min, max);
	return proj;
}

Projection& Boundary::ProjectingPoint(const Vector3 &point, const Vector3 &axis)
{
	float min, max;
	min = max = axis.Dot(point);
	Projection proj(min, max);
	return proj;
}