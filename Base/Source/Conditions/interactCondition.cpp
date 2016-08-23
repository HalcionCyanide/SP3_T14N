#include "interactCondition.h"

interactCondition::interactCondition()
	//: theRequirement(nullptr)
{
	//target = nullptr;
}

interactCondition::interactCondition(std::string)
	//: theRequirement(nullptr)
{
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
	if (!satisfied)
	{
		//if (theRequirement.value_->getInteracting())
		{
			satisfied = true;
		}
	}
	else
	{
		return;
	}
}