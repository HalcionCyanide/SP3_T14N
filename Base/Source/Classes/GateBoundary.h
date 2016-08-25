#ifndef GATEBOUNDARY_H
#define GATEBOUNDARY_H

#include "Boundary.h"
#include "..//Systems/Scene_System.h"

class GateBoundary : public Boundary
{
public:
	GateBoundary();
	GateBoundary(const std::string &ID);
	virtual ~GateBoundary();

	virtual void SetIDValue(const std::string &ID);
	
	std::string GetIDValue()const;

	virtual float GetOverlappingDistance()const;
	virtual bool CheckCollision(const Vector3 &point);
	virtual void TransitScene(const std::string &ID);

private:
	std::string IDValue;
};


#endif