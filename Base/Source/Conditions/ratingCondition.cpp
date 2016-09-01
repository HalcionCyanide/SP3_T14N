#include "ratingCondition.h"
#include "..\\Systems\\Scene_System.h"

ratingCondition::ratingCondition()
	: theRequirement("")
{
}

ratingCondition::ratingCondition(std::string i)
	: theRequirement(i)
{
}

void ratingCondition::setValue(std::string i)
{
	theRequirement = std::stoi(i);
}

ratingCondition::~ratingCondition()
{
}

void ratingCondition::Update(double dt)
{
	//stuff to increase the number, please remember to increase the counter as you go along.
	/* 
	//some pseudocode*/
	if(!complete)
	{
		counter = Scene_System::accessing().gPlayer->GetSpellPower();
		if (counter >= stoi(theRequirement))
		{
			complete = true;
		}
	}
	else
	{
		return;
	}
}