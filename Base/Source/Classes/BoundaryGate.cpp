#include "BoundaryGate.h"
#include "../Systems/Scene_System.h"

BoundaryGate::BoundaryGate()
{

}

BoundaryGate::~BoundaryGate()
{

}

bool BoundaryGate::CheckCollision(const Boundary &object)
{
    for (int i = 0; i < this->VerticeNo; i++)
    {
        Projection point1 = SetProjection(this->Axis[i], this->Vertices);
        Projection point2 = SetProjection(this->Axis[i], object.GetVertices());
        if (!point1.DetermineCollision(point2))
            return false;
    }
    for (int i = 0; i < object.GetVerticeNo(); i++)
    {
        Projection point1 = SetProjection(object.GetAxis()[i], this->Vertices);
        Projection point2 = SetProjection(object.GetAxis()[i], object.GetVertices());
        if (!point1.DetermineCollision(point2))
            return false;
    }
    return true;
}

bool BoundaryGate::CheckCollision(const Vector3 &point)
{
    SetOverlappingDistance(0.f);
    SetOverlappingAxis(Vector3(0, 0, 0));
    float overlap = 9999.f;
    Vector3 overlappedAxis;
    for (int i = 0; i < VerticeNo; i++)
    {
        Projection point1 = SetProjection(this->Axis[i], this->Vertices);
        Projection point2 = SetProjectionPoint(this->Axis[i], point);
        if (!point1.DetermineCollision(point2))
            return false;
        else
        {
            float value = point1.GetOverlappingDistance(point2);
            if (overlap > value)
            {
                overlap = value;
                overlappedAxis = this->Axis[i];
            }
        }
    }
    SetOverlappingDistance(overlap);
    SetOverlappingAxis(overlappedAxis);
    Scene_System::accessing().SwitchScene(name_);
    return true;
}

float BoundaryGate::GetOverlappingDistance()const
{
    return this->OverlappingDistance * 10.f;
}