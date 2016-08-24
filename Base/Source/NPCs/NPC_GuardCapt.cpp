#include "NPC_GuardCapt.h"
#include "..\\Systems\\Scene_System.h"

GuardCapt::GuardCapt()
{
}

GuardCapt::~GuardCapt()
{
}

void GuardCapt::Init()
{
	name_ = "NPC_guardcaptain";

	for (std::vector<Quest*>::iterator it = Scene_System::accessing().QM.allQuests.begin(); it != Scene_System::accessing().QM.allQuests.end(); ++it)
	{
		Quest* q = (Quest*)*it;

		if (q->getGiver() == name_)
		{
			myQuests.push_back(q);
		}
	}
}

void GuardCapt::Update(double dt)
{
	static float currentAngle = 0.f;
	float desiredAngle = Math::RadianToDegree(atan2(getTarget().x - GetPosition().x, getTarget().z - GetPosition().z));
	if ((getTarget() - GetPosition()).LengthSquared() < 900)
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
}