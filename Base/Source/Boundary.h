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

	virtual bool CheckCollision(const Boundary &object)const;
	virtual bool CheckCollision(const Vector3 &point)const;
	//virtual bool CheckCollision2D(const Vector3 &point)const;
	virtual void CalculateValues(const Vector3 &pos, const Vector3 &scale);

	virtual void SetPosition(const Vector3 &position);
	virtual void SetScale(const Vector3 &scale);

	Vector3 GetPosition()const;
	Vector3 GetScale()const;
private:
	Vector3 Position, Scale;
};








#endif