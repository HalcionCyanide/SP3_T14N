#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "Vector3.h"
#include "Mtx44.h"
#include "Projection.h"

class Boundary
{
public:
	Boundary();
	Boundary(const Vector3 &pos, const Vector3 &dimension, const float &rotationValue = 0);
	Boundary(const Boundary &bounds);
	virtual ~Boundary();

	//virtual bool CheckCollision( Boundary &object);
	//virtual bool CheckCollision(const Vector3 &point);
	//virtual void CalculateValues(const Vector3 &pos, const Vector3 &scale, const float &rotation = 0);

	//virtual void SetPosition(const Vector3 &position);
	//virtual void SetScale(const Vector3 &scale);
	//virtual void SetRotation(const float &rotationValue);

	//Vector3 GetPosition()const;
	//Vector3 GetScale()const;
	//float GetRotation()const;

	//virtual void SetAxes();
	//virtual void SetVertices();
	//virtual void SetVerticeNo(const int &value);

	//Vector3 GetVerticeArray()const;

	//Projection& Projecting(const Vector3 &axis);
	//Projection& ProjectingPoint(const Vector3 &point, const Vector3 &axis);

	virtual void SetPosition(const Vector3 &position);
	virtual void SetDimension(const Vector3 &scale);
	virtual void SetRotation(const float &rotationValue);
	virtual void SetVerticeNo(const int &verticeNo);
	virtual void SetAxis();
	virtual void SetVertices();

	Vector3 GetPosition()const;
	Vector3 GetDimension()const;
	float GetRotation()const;
	int GetVerticeNo()const;
	Vector3* GetAxis()const;
	Vector3* GetVertices()const;

	bool CheckCollision(const Boundary &object);
	bool CheckCollision(const Vector3 &point);

	void ResetValues(const Vector3 &position, const Vector3 &dimension, const float &rotation = 0);
	
	Projection SetProjection(const Vector3 &axis, Vector3* vertice);
	Projection SetProjectionPoint(const Vector3 &axis, const Vector3 &point);

protected:
	//Vector3* Axes;
	//Vector3* Vertices;
	//int VerticesNo = 4;

	//float RotationValue;
	//Vector3 Position, Scale;

	Vector3 Position, Dimension;
	float Rotation;
	int VerticeNo;
	Vector3* Axis;
	Vector3* Vertices;
};
#endif