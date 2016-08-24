#ifndef FREEFIELDGATEBOUNDARY_H
#define FREEFIELDGATEBOUNDARY_H

#include "Boundary.h"

class FreeFieldGateBoundary : public Boundary
{
public:
	FreeFieldGateBoundary();
	virtual ~FreeFieldGateBoundary();

    virtual bool CheckCollision(const Boundary &object);
    virtual bool CheckCollision(const Vector3 &point);
    virtual float GetOverlappingDistance()const;
};

#endif