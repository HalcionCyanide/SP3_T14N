#include "Boundary.h"

Boundary::Boundary()
{
	//Vertices = nullptr;
	//Axes = nullptr;
	Axis = nullptr;
	Vertices = nullptr;
	SetVerticeNo(4);
}

Boundary::Boundary(const Vector3 &pos, const Vector3 &dimension, const float &rotation)
{
	//Vertices = nullptr;
	//Axes = nullptr;
	//CalculateValues(pos, scale, rotation);
	Axis = nullptr;
	Vertices = nullptr;
	SetVerticeNo(4);
	ResetValues(pos, dimension, rotation);
}

Boundary::Boundary(const Boundary &bounds)
{
	this->Position = bounds.Position;
	this->Dimension = bounds.Dimension;
	this->Rotation = bounds.Rotation;
	this->Axis = bounds.Axis;
	this->Vertices = bounds.Vertices;
	this->VerticeNo = bounds.VerticeNo;
}

Boundary::~Boundary()
{
	if (Axis)
	{
		delete []Axis;
		Axis = nullptr;
	}
	if (Vertices)
	{
		delete[]Vertices;
		Vertices = nullptr;
	}
}
//
//bool Boundary::CheckCollision(Boundary &object)
//{
//	//for (int i = 0; i < VerticesNo; ++i)
//	//{
//	//	Projection* point1 = &Projecting(this->Axes[i]);
//	//	Projection* point2 = &object.Projecting(this->Axes[i]);
//	//	if (!point1->DetermineCollision(*point2))
//	//		return false;
//	//}
//	//for (int i = 0; i < VerticesNo; ++i)
//	//{
//	//	Projection* point1 = &Projecting(object.Axes[i]);
//	//	Projection* point2 = &object.Projecting(object.Axes[i]);
//	//	if (!point1->DetermineCollision(*point2))
//	//		return false;
//	//}
//	//return true;
//}
//
//bool Boundary::CheckCollision(const Vector3 &point)
//{
//	//for (int i = 0; i < VerticesNo; ++i)
//	//{
//	//	Projection* point1 = &Projecting(this->Axes[i]);
//	//	Projection* point2 = &ProjectingPoint(point, this->Axes[i]);
//	//	if (point1->DetermineCollision(*point2))
//	//		return false;
//	//}
//	//return true;
//}
//
//void Boundary::CalculateValues(const Vector3 &pos, const Vector3 &scale, const float &rotation)
//{
//	//SetPosition(pos);
//	//SetScale(scale + Vector3(5, 5, 5));
//	//SetRotation(rotation);
//	//SetVertices();
//	//SetAxes();
//}
//
//void Boundary::SetPosition(const Vector3 &position)
//{
//	//this->Position = position;
//}
//
//void Boundary::SetScale(const Vector3 &scale)
//{
//	//this->Scale = scale;
//}
//
//void Boundary::SetRotation(const float &rotation)
//{
//	//this->RotationValue = rotation;
//}
//
//Vector3 Boundary::GetPosition()const
//{
//	//return Position;
//}
//
//Vector3 Boundary::GetScale()const
//{
//	//return Scale;
//}
//
//void Boundary::SetAxes()
//{
//	//if (Axes)
//	//{
//	//	delete[]Axes;
//	//	Axes = nullptr;
//	//}
//	//this->Axes = new Vector3[VerticesNo];
//	//for (int i = 0; i < VerticesNo; ++i)
//	//{
//	//	Vector3 point1 = Vertices[i];
//	//	Vector3 point2 = Vertices[i + 1 == VerticesNo ? 0 : i + 1];
//	//	Vector3 N = point1.Cross(point2);
//	//	Axes[i] = N;
//	//	Axes[i].Normalize();
//	//}
//}
//
//void Boundary::SetVertices()
//{
//	////Currently limit to 4 Vertices only, will need to change if have more
//	//if (Vertices)
//	//{
//	//	delete[]Vertices;
//	//	Vertices = nullptr;
//	//}
//	//Vertices = new Vector3[VerticesNo];
//	//Mtx44 Rotation;
//	//Rotation.SetToRotation(this->RotationValue, 0, 1, 0);
//	//Vector3 Scale = Rotation * this->Scale;
//	//Vertices[0] = Vector3(this->Position.x + Scale.x/2, 0, this->Position.z + Scale.z/2);
//	//Vertices[1] = Vector3(this->Position.x + Scale.x/2, 0, this->Position.z - Scale.z/2);
//	//Vertices[2] = Vector3(this->Position.x - Scale.x/2, 0, this->Position.z - Scale.z/2);
//	//Vertices[3] = Vector3(this->Position.x - Scale.x/2, 0, this->Position.z + Scale.z/2);
//}
//
//void Boundary::SetVerticeNo(const int &value)
//{
//	//this->VerticesNo = value;
//}
//
//Projection& Boundary::Projecting(const Vector3 &axis)
//{
//	//float min, max;
//	//min = max = axis.Dot(this->Vertices[0]);
//	//for (int i = 1; i < VerticesNo; ++i)
//	//{
//	//	float value = axis.Dot(this->Vertices[i]);
//	//	if (value < min)
//	//		min = value;
//	//	else if (value > max)
//	//		max = value;
//	//}
//	//Projection proj(min, max);
//	//return proj;
//}
//
//Projection& Boundary::ProjectingPoint(const Vector3 &point, const Vector3 &axis)
//{
//	//float min, max;
//	//min = max = axis.Dot(point);
//	//Projection proj(min, max);
//	//return proj;
//}
//


void Boundary::SetPosition(const Vector3 &position)
{
	this->Position = position;
}

void Boundary::SetDimension(const Vector3 &dimension)
{
	this->Dimension = dimension;
}

void Boundary::SetRotation(const float &rotation)
{
	this->Rotation = rotation;
}

void Boundary::SetVerticeNo(const int &verticeNo)
{
	this->VerticeNo = verticeNo;
}

void Boundary::SetAxis()
{
	if (Axis)
	{
		delete[]Axis;
		Axis = nullptr;
	}
	Axis = new Vector3[VerticeNo];
	for (int i = 0; i < VerticeNo; ++i)
	{
		Vector3 point1 = Vertices[i];
		Vector3 point2 = Vertices[i + 1 == VerticeNo ? 0 : i + 1];
		Vector3 edge = point1 - point2;
		Vector3 N = Vector3(-edge.z, 0, edge.x);
		N.Normalize();
		Axis[i] = N;
	}
}

void Boundary::SetVertices()
{
	if (Vertices)
	{
		delete[]Vertices;
		Vertices = nullptr;
	}
	Vertices = new Vector3[VerticeNo];
	Mtx44 Rotation;
	Rotation.SetToRotation(this->Rotation, 0, 1, 0);
	Vector3 dimension = Rotation * this->Dimension;
	Vertices[0] = Vector3(this->Position.x + dimension.x / 2, 0, this->Position.z + dimension.z / 2);
	Vertices[1] = Vector3(this->Position.x + dimension.x / 2, 0, this->Position.z - dimension.z / 2);
	Vertices[2] = Vector3(this->Position.x - dimension.x / 2, 0, this->Position.z - dimension.z / 2);
	Vertices[3] = Vector3(this->Position.x - dimension.x / 2, 0, this->Position.z + dimension.z / 2);
}

Vector3 Boundary::GetPosition()const
{
	return this->Position;
}

Vector3 Boundary::GetDimension()const
{
	return this->Dimension;
}

float Boundary::GetRotation()const
{
	return this->Rotation;
}

int Boundary::GetVerticeNo()const
{
	return this->VerticeNo;
}

Vector3* Boundary::GetAxis()const
{
	return this->Axis;
}

Vector3* Boundary::GetVertices()const
{
	return this->Vertices;
}

bool Boundary::CheckCollision(const Boundary &object)
{
	for (int i = 0; i < this->VerticeNo; ++i)
	{
		Projection point1 = SetProjection(this->Axis[i], this->Vertices);
		Projection point2 = SetProjection(this->Axis[i], object.Vertices);
		if (!point1.DetermineCollision(point2))
			return false;
	}
	for (int i = 0; i < object.VerticeNo; ++i)
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
	for (int i = 0; i < Axis->Length(); ++i)
	{
		Projection point1 = SetProjection(this->Axis[i], this->Vertices);
		Projection point2 = SetProjectionPoint(this->Axis[i], point);
		if (!point1.DetermineCollision(point2))
			return false;
	}
	return true;
}

void Boundary::ResetValues(const Vector3 &position, const Vector3 &dimension, const float &rotation)
{
	SetPosition(position);
	SetDimension(dimension);
	SetRotation(rotation);
	SetVertices();
	SetAxis();
}

Projection Boundary::SetProjection(const Vector3 &axis, Vector3* vertice)
{
	float min, max;
	min = max = (axis.x * vertice[0].x + axis.z * vertice[0].z);
	for (int i = 1; i < this->VerticeNo; ++i)
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
	min = max = axis.Dot(point);
	Projection proj(min, max);
	return proj;
}