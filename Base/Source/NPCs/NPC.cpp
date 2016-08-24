#include "NPC.h"
#include "..\\Systems\\Scene_System.h"

NPC::NPC()
	:interacting(false)
	, flavourText("")
	, target(Vector3(0,0,0))
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
	static float currentAngle = 0.f;
	float desiredAngle = Math::RadianToDegree(atan2(target.x - GetPosition().x, target.z - GetPosition().z));
	if ((target - GetPosition()).LengthSquared() < 900)
	{
		float Speed = 50.f;

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

	if (Scene_System::accessing().cSS_InputManager->GetKeyValue('Q') && !interacting)
	{
		interacting = true;
	}
}