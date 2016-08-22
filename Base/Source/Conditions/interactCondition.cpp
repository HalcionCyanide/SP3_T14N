#include "interactCondition.h"

interactCondition::interactCondition()
	: theRequirement(nullptr)
{
	//target = nullptr;
}

interactCondition::interactCondition(NPC* i)
	: theRequirement(nullptr)
{
	theRequirement = i;
}

interactCondition::~interactCondition()
{
}

void interactCondition::Update(double dt)
{
	//stuff to increase the number, please remember to increase the counter as you go along.
	/*<!>
	//some pseudocode*/
	if (!satisfied)
	{
		if (theRequirement.value_->getINTT())
		{
			satisfied = true;
		}
	}
	else
	{
		return;
	}
}