#include "ratingCondition.h"
#include "..\\Systems\\Scene_System.h"

ratingCondition::ratingCondition()
	: theRequirement(0)
	, counter(0)
{
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

void ratingCondition::Update(double dt)
{
	//stuff to increase the number, please remember to increase the counter as you go along.
	/*<!> 
	//some pseudocode*/
	if(!complete)
	{
		counter = Scene_System::accessing().gPlayer->GetSpellPower();
		if (theRequirement <= counter)
		{
			complete = true;
		}
	}
	else
	{
		return;
	}
}