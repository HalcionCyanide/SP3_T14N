#include "ratingCondition.h"

ratingCondition::ratingCondition()
	: theRequirement(0)
	, counter(0)
{
	//target = nullptr;
}

ratingCondition::ratingCondition(int i)
	: theRequirement(0)
	, counter(0)
{
	theRequirement = i;
}

ratingCondition::~ratingCondition()
{
}

void ratingCondition::setValue(std::string i)
{
	theRequirement = stoi(i);
}

void ratingCondition::Update(double dt)
{
	//stuff to increase the number, please remember to increase the counter as you go along.
	/*<!> 
	//some pseudocode*/
	if(!satisfied)
	{
		/*counter = player.rating;*/
		if (theRequirement <= counter)
		{
			satisfied = true;
		}
	}
	else
	{
		return;
	}
}