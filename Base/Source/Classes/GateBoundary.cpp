#include "GateBoundary.h"

GateBoundary::GateBoundary()
{
}

GateBoundary::~GateBoundary()
{
}

float GateBoundary::GetOverlappingDistance()const
{
	return this->OverlappingDistance * 75.f;
}

bool GateBoundary::CheckCollision(const Vector3 &point)
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
	TransitScene(name_);
	return true;
}

void GateBoundary::TransitScene(const std::string &ID)
{
	Scene_System::accessing().SwitchScene(ID);
    Scene_System::accessing().gPlayer->currSceneID = ID;
    Scene_System::accessing().getCurrScene().onNotify("TRANSITIONING");
}