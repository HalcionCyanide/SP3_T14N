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
	//UI STUFF?!
	//ANYTHING HERE?!

	for (std::vector<Quest*>::iterator it = myQuests.begin(); it != myQuests.end(); ++it)
	{
		Quest* q = (Quest*)*it;
		if (q->getActive())
		{
			q->Update(dt);
		}
	}
}