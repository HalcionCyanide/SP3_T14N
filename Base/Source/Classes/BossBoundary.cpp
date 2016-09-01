#include "BossBoundary.h"

BossBoundary::BossBoundary()
{
}

BossBoundary::~BossBoundary()
{
}

float BossBoundary::GetOverlappingDistance()const
{
	return this->OverlappingDistance * 75.f;
}

bool BossBoundary::CheckCollision(const Vector3 &point)
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
	
	std::ostringstream ss;
	ss << "BOSSMONSTER_" << name_;
	Scene_System::accessing().getCurrScene().onNotify(ss.str());	
	return true;
}
