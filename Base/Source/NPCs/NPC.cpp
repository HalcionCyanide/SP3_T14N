#include "NPC.h"
#include "..\\Systems\\Scene_System.h"

NPC::NPC()
	:interacting(false)
	, flavourText("")
	, target(Vector3(0,0,0))
	, currentAngle(0.f)
	, DetectionRadiusSquared(300)
{
}

NPC::~NPC()
{
}

std::string NPC::getFText()
{
	return flavourText;
}

void NPC::setFText(std::string i)
{
	flavourText = i;
}

bool NPC::getInteracting()
{
	return interacting;
}

void NPC::setInteracting(bool i)
{
	interacting = i;
}

Vector3 NPC::getTarget()
{
	return target;
}

void NPC::setTarget(const Vector3& i)
{
	target = i;
}

void NPC::Update(double dt)
{
	float desiredAngle = (float)(((int)Math::RadianToDegree(atan2(target.x - GetPosition().x, target.z - GetPosition().z))) % 360);
	if ((target - GetPosition()).LengthSquared() < DetectionRadiusSquared)
	{
		float Speed = 100.f;

		if (currentAngle + Speed * dt < desiredAngle + Math::EPSILON)
		{
			currentAngle += Speed * (float)dt;
		}
		if (currentAngle - Speed * dt > desiredAngle - Math::EPSILON)
		{
			currentAngle -= Speed * (float)dt;
		}

		SetRotationAngle(currentAngle);
	}
}

void NPC::SetDetectionRadiusSquared(const float& DRS)
{
	DetectionRadiusSquared = DRS;
}

float NPC::GetDetectionRadiusSquared()
{
	return DetectionRadiusSquared;
}