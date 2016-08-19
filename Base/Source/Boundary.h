#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "Vector3.h"
#include "Mtx44.h"

class Boundary
{
public:
	Boundary(){};
	Boundary(const Vector3 &pos, const Vector3 &scale, const float &rotationValue = 0);
	Boundary(const Boundary &bounds);
	virtual ~Boundary(){};

	virtual bool CheckCollision(const Boundary &object)const;
	virtual bool CheckCollision(const Vector3 &point, const Vector3 &vel)const;
	virtual void CalculateValues(const Vector3 &pos, const Vector3 &scale, const float &rotation = 0);

	virtual void SetPosition(const Vector3 &position);
	virtual void SetScale(const Vector3 &scale);
	virtual void SetRotation(const float &rotationValue);
	virtual void SetNormal();

	Vector3 GetPosition()const;
	Vector3 GetScale()const;
	Vector3 GetFlipScale()const;
	float GetRotation()const;
	Vector3 GetNormal()const;
	Vector3 GetFlipNormal()const;
private:
	float RotationValue;
	Vector3 Position, Scale, Normal;
	Vector3 FlipScale, FlipNormal;
};








#endif