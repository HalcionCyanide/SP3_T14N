#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "Vector3.h"

class Boundary
{
public:
	Boundary(){};
	Boundary(const Vector3 &pos, const Vector3 &scale);
	Boundary(const Boundary &bounds);
	virtual ~Boundary(){};

	virtual bool CheckCollision(const Boundary &object, const Boundary &player)const;
	virtual bool CheckCollision2D(const Boundary &object, const Vector3 &point)const;
	virtual void CalculateValues(const Vector3 &pos, const Vector3 &scale);

	virtual void SetMaxValues(const Vector3 &maxValue);
	virtual void SetMinValues(const Vector3 &minValue);

	Vector3 GetMaxValues()const;
	Vector3 GetMinValues()const;

private:
	Vector3 MaxValues, MinValues;
};








#endif