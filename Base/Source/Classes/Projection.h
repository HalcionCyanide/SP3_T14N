#ifndef PROJECTION_H
#define PROJECTION_H

#include "Vector3.h"

class Projection
{
public:
	Projection(){};
	Projection(const float &min, const float &max);
	Projection(const Projection &proj);
	~Projection();

	virtual void SetMin(const float &min);
	virtual void SetMax(const float &max);

	float GetMin()const;
	float GetMax()const;

	bool DetermineCollision(const Projection &value);
	bool DetermineCollision(const float &value);

	float GetOverlappingDistance(const Projection &value)const;

private:
	float min;
	float max;
};
#endif // !PROJECTION_H