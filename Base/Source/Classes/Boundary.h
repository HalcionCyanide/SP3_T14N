#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "Vector3.h"
#include <vector>
#include "Mtx44.h"
#include "Projection.h"
#include "BaseObject.h"

class Boundary : public BaseObject
{
public:
	Boundary();
	Boundary(const Vector3 &pos, const Vector3 &dimension, const float &rotationValue = 0);
	Boundary(const Boundary &bounds);
	virtual ~Boundary();

	virtual void SetVerticeNo(const int &verticeNo);
	virtual void SetAxis();
	virtual void SetVertices();

	int GetVerticeNo()const;
	std::vector<Vector3> GetAxis()const;
	std::vector<Vector3> GetVertices()const;

	bool CheckCollision(const Boundary &object);
	bool CheckCollision(const Vector3 &point);

	void ResetValues();
	
	Projection SetProjection(const Vector3 &axis, std::vector<Vector3> vertice);
	Projection SetProjectionPoint(const Vector3 &axis, const Vector3 &point);

	virtual void SetOverlappingAxis(const Vector3 &axis);
	virtual void SetOverlappingDistance(const float &distance);

	Vector3 GetOverlappingAxis()const;
	float GetOverlappingDistance()const;

protected:
	int VerticeNo;
	std::vector<Vector3> Axis;
	std::vector<Vector3> Vertices;
	Vector3 OverlappingAxis;
	float OverlappingDistance;
};
#endif