#include "gotoCondition.h"


gotoCondition::gotoCondition()
	: theRequirement(0)
	, dist(0)
{
}

gotoCondition::gotoCondition(int i)
	: theRequirement(0)
	, dist(i)
{
}

gotoCondition::~gotoCondition()
{
}

void gotoCondition::Update(double dt)
{
	//stuff to increase the number, please remember to increase the counter as you go along.
	if (!satisfied)
	{
		//if ((Player.pos - (*theRequirement.value_)).LengthSquared() < dist)
		{
			satisfied = true;
		}
	}
	else
	{
		return;
	}
}