#ifndef BOUNDARY_GATE_H
#define BOUNDARY_GATE_H

#include "Boundary.h"

class BoundaryGate : public Boundary
{
public:
    BoundaryGate();
    virtual ~BoundaryGate();

    virtual bool CheckCollision(const Boundary &object);
    virtual bool CheckCollision(const Vector3 &point);

    virtual float GetOverlappingDistance()const;
};

#endif