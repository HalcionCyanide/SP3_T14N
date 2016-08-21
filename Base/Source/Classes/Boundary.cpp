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

bool Boundary::CheckCollision(const Boundary &object)
{
	for (int i = 0; i < VerticesNo; ++i)
	{
		if (!this->Projected[i].DetermineCollision(object.Projected[i]))
			return false;
	}
	return true;
}

bool Boundary::CheckCollision(const Vector3 &point, const Vector3 &scale)
{
	return false;
}

void Boundary::CalculateValues(const Vector3 &pos, const Vector3 &scale, const float &rotation)
{
	SetPosition(pos);
	SetScale(scale + Vector3(5, 5, 5));
	SetRotation(rotation);
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

void Boundary::SetAxes(const Vector3 &pos, const Vector3 &scale, const float &rotation)
{
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
	Vertices = new Vector3[VerticesNo];
	Mtx44 Rotation;
	Rotation.SetToRotation(this->RotationValue, 0, 1, 0);
	Vector3 Scale = Rotation * this->Scale;
	Vertices[0] = Vector3(this->Position.x + Scale.z, this->Position.y, this->Position.z + Scale.z);
	Vertices[1] = Vector3(this->Position.x + Scale.z, this->Position.y, this->Position.z - Scale.z);
	Vertices[2] = Vector3(this->Position.x - Scale.z, this->Position.y, this->Position.z - Scale.z);
	Vertices[3] = Vector3(this->Position.x - Scale.z, this->Position.y, this->Position.z + Scale.z);
}

void Boundary::SetProjection(const Projection &value)
{
	Projected = new Projection[VerticesNo];
	for (int i = 0; i < VerticesNo; ++i)
	{
		Projected[i] = *Projecting(Axes[i]);
	}
}


Projection* Boundary::Projecting(const Vector3 &axis)
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
	Projection* proj = new Projection(min, max);
	return proj;
}