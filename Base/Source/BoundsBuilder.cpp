#include "BoundsBuilder.h"

Boundary *BoundsBuilder::BuildSquareBoundary(const Vector3 &size, const Vector3 &pos)
{
    Boundary *theBounds = new Boundary(pos, size);
    return theBounds;
}