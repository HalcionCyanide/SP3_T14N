#include "Boundary.h"

Boundary::Boundary() : BaseObject()
{
	SetVerticeNo(4);
}

Boundary::Boundary(const Vector3 &pos, const Vector3 &dimension, const float &rotation)
{
	SetPosition(pos);
	SetDimensions(dimension);
	SetRotationAngle(rotation);
	SetVerticeNo(4);
	ResetValues();
}

Boundary::Boundary(const Boundary &bounds)
{
	this->Axis = bounds.Axis;
	this->Vertices = bounds.Vertices;
	this->VerticeNo = bounds.VerticeNo;
}

Boundary::~Boundary()
{
	Axis.clear();
	Vertices.clear();
}

void Boundary::SetVerticeNo(const int &verticeNo)
{
	this->VerticeNo = verticeNo;
}

void Boundary::SetAxis()
{
	Axis.clear();
	for (int i = 0; i < VerticeNo; ++i)
	{
		Vector3 point1 = Vertices[i];
		Vector3 point2 = Vertices[i + 1 == VerticeNo ? 0 : i + 1];
		Vector3 edge = point1 - point2;
		Vector3 N = Vector3(-edge.z, 0, edge.x);
		//N.Normalize();
		Axis.push_back(N);
	}
}

void Boundary::SetVertices()
{
	Vertices.clear();
	Mtx44 Rotation;
	Rotation.SetToRotation(this->GetRotationAngle(), 0, 1, 0);
	Vector3 dimension = Rotation * this->GetDimensions();
	Vertices.push_back(Vector3(this->GetPosition().x + dimension.x / 2, 0, this->GetPosition().z + dimension.z / 2));
	Vertices.push_back(Vector3(this->GetPosition().x + dimension.x / 2, 0, this->GetPosition().z - dimension.z / 2));
	Vertices.push_back(Vector3(this->GetPosition().x - dimension.x / 2, 0, this->GetPosition().z - dimension.z / 2));
	Vertices.push_back(Vector3(this->GetPosition().x - dimension.x / 2, 0, this->GetPosition().z + dimension.z / 2));
	Rotation.SetToZero();
}

int Boundary::GetVerticeNo()const
{
	return this->VerticeNo;
}

std::vector<Vector3> Boundary::GetAxis()const
{
	return this->Axis;
}

std::vector<Vector3> Boundary::GetVertices()const
{
	return this->Vertices;
}

bool Boundary::CheckCollision(const Boundary &object)
{
	for (int i = 0; i < this->VerticeNo; i++)
	{
		Projection point1 = SetProjection(this->Axis[i], this->Vertices);
		Projection point2 = SetProjection(this->Axis[i], object.Vertices);
		if (!point1.DetermineCollision(point2))
			return false;
	}
	for (int i = 0; i < object.VerticeNo; i++)
	{
		Projection point1 = SetProjection(object.Axis[i], this->Vertices);
		Projection point2 = SetProjection(object.Axis[i], object.Vertices);
		if (!point1.DetermineCollision(point2))
			return false;
	}
	return true;
}

bool Boundary::CheckCollision(const Vector3 &point)
{
	for (int i = 0; i < VerticeNo; i++)
	{
		Projection point1 = SetProjection(this->Axis[i], this->Vertices);
		Projection point2 = SetProjectionPoint(this->Axis[i], point);
		if (!point1.DetermineCollision(point2))
			return false;
	}
	return true;
}

void Boundary::ResetValues()
{
	SetVertices();
	SetAxis();
}

Projection Boundary::SetProjection(const Vector3 &axis, std::vector<Vector3> vertice)
{
	float min, max;
	min = max = axis.Dot(vertice[0]);
	for (int i = 1; i < this->VerticeNo; i++)
	{
		float value = (axis.x * vertice[i].x + axis.z * vertice[i].z);
		if (value < min)
			min = value;
		else if (value > max)
			max = value;
	}
	Projection proj(min, max);
	return proj;
}

Projection Boundary::SetProjectionPoint(const Vector3 &axis, const Vector3 &point)
{
	float min, max;
	min = max = (axis.x * point.x + axis.z * point.z);
	Projection proj(min, max);
	return proj;
}