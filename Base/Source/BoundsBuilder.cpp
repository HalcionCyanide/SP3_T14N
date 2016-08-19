#include "BoundsBuilder.h"

Boundary *BoundsBuilder::BuildSquareBoundary(const Vector3 &size, const Vector3 &pos, const float &rotationAngle)
{
    Boundary *theBounds = new Boundary(pos, size, rotationAngle);
    return theBounds;
}