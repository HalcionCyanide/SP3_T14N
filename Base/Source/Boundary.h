#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "GameObject.h"

class Boundary
{
public:
	Boundary(){};
	Boundary(GameObject* object){
		StoreValues(object);
	}
	Boundary(const Boundary* &bounds){
		if (this != bounds)
		{
			this->MaxValues = bounds->MaxValues;
			this->MinValues = bounds->MinValues;
		}
	}
	virtual ~Boundary();
	virtual bool CheckCollision(const Boundary* &object, const Boundary* &player);
	virtual bool CheckCollision2D(const Boundary* &object, const Vector3 &point);
	virtual void CalculateValues(const GameObject* &object);
	virtual void SetMaxValues(const Vector3 &maxValue);
	virtual void SetMinValues(const Vector3 &minValue);

	Vector3 GetMaxValues();
	Vector3 GetMinValues();
private:
	Vector3 MaxValues, MinValues;

};








#endif