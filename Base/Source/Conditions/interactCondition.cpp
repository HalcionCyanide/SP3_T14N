#include "interactCondition.h"
#include "..\\Systems\\Scene_System.h"

interactCondition::interactCondition()
	: theRequirement(nullptr)
{
}

interactCondition::interactCondition(std::string i)
	: theRequirement(nullptr)
{
	for (auto it : Scene_System::accessing().NM.allNPCs)
	{
		if (it->getName() == i)
		{
			theRequirement = it;
		}
	}
}

interactCondition::~interactCondition()
{
}

void interactCondition::setValue(std::string)
{

}

void interactCondition::Update(double dt)
{
	//stuff to increase the number, please remember to increase the counter as you go along.
	/*<!>
	//some pseudocode*/
	if (!complete)
	{
		if (theRequirement.value_->getInteracting())
		{
			complete = true;
		}
	}
	else
	{
		return;
	}
}