#ifndef TOWN1GATEBOUNDARY_H
#define TOWN1GATEBOUNDARY_H

#include "Boundary.h"

class Town1GateBoundary : public Boundary
{
public:
	Town1GateBoundary();
	virtual ~Town1GateBoundary();

    virtual bool CheckCollision(const Boundary &object);
    virtual float GetOverlappingDistance()const;
};

#endif