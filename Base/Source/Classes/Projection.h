#ifndef PROJECTION_H
#define PROJECTION_H

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
private:
	float min;
	float max;
};
#endif // !PROJECTION_H