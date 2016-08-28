#include "BattleScreenObject.h"

BattleScreenObject::BattleScreenObject(const std::string& MeshName, const float& Mass, const Vector3& Position, const Vector3& Dimensions, const Vector3& Acceleration, const float& RotationAngle, const Vector3& RotationAxis)
{
	Init(MeshName, Mass, Position, Dimensions, 0, RotationAngle, RotationAxis);
	SetAcceleration(Acceleration);
}

BattleScreenObject::~BattleScreenObject()
{
	Exit();
}

// Virtual
void BattleScreenObject::Init(const std::string& MeshName, const float& Mass, const Vector3& Position, const Vector3& Dimensions, const Vector3& Acceleration, const float& RotationAngle, const Vector3& RotationAxis)
{
	Active = true;
	Visible = true;
	SetParameters(MeshName, Mass, Position, Dimensions, 0, RotationAngle, RotationAxis);
	SetAcceleration(Acceleration);
}

void BattleScreenObject::Update(double dt)
{
	LifeTimer += (float)dt;
	if (LifeTime != -1)
		if (LifeTimer > LifeTime)
			Active = false;
	if (MoveToTarget)
	{
		SetAcceleration(0);
		Vector3 DVec = TargetPoint - GetPosition();
		SetVelocity(DVec);
		if (DVec.LengthSquared() < 10.f)
		{
			if (AltTargetPoint != 0)
			{
				SetVelocity(0);
 				std::swap(TargetPoint, AltTargetPoint);
				DVec = TargetPoint - GetPosition();
				SetAcceleration(DVec);
				MoveToTarget = false;
			}
			else MoveToTarget = false;
		}
	}
	if (Active) // Still can update if invisible
	{
		switch (Type)
		{
		case BS_Bullet:
		{
			Vector3 StoredVelocity = GetVelocity();
			if (GravityAffected)
				SetVelocity(GetVelocity() + Vector3(0, -9.8f) * (float)dt); // For Gravity
			if (GetMass() > Math::EPSILON)
			{
				SetVelocity(GetVelocity() + Acceleration * (1.f / GetMass()) * (float)dt);
			}
			SetPosition(GetPosition() + (StoredVelocity + GetVelocity()) * 0.5f * (float)dt);
			SetRotationAngle(Math::RadianToDegree(atan2(-GetVelocity().x, GetVelocity().y)));
			break;
		}
		case BS_Blast:
		{
			if (LifeTimer > (LifeTime * 0.25f))
			{
				Visible = true;
				SetRotationAngle(GetRotationAngle() + GetRotationAngle() * (float)dt * (float)dt);
				SetDimensions(GetDimensions() + GetVelocity().LengthSquared() * GetDimensions() * (float)dt  * (float)dt);
			}
			break;
		}
		default:
		{
			Vector3 StoredVelocity = GetVelocity();
			if (GravityAffected)
				SetVelocity(GetVelocity() + Vector3(0, -9.8f) * (float)dt); // For Gravity
			if (GetMass() > Math::EPSILON)
			{
				SetVelocity(GetVelocity() + Acceleration * (1.f / GetMass()) * (float)dt);
			}
			SetPosition(GetPosition() + (StoredVelocity + GetVelocity()) * 0.5f * (float)dt);
			//SetRotationAngle(Math::RadianToDegree(atan2(-GetVelocity().x, GetVelocity().y)));
			break;
		}
		}
	}
}

void BattleScreenObject::SetAcceleration(const Vector3& A)
{
	Acceleration = A;
}

Vector3 BattleScreenObject::GetAcceleration()
{
	return Acceleration;
}
