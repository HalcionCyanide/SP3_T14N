#pragma once

#include "Boundary.h"

class BoundsBuilder
{
public:
    static Boundary *BuildSquareBoundary(const Vector3 &size, const Vector3 &pos, const float &rotationAngle);
};