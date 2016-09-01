#include "gotoCondition.h"
#include "..\\Systems\\Scene_System.h"
#include <sstream>

gotoCondition::gotoCondition()
	: theRequirement("")
{
}

gotoCondition::gotoCondition(std::string i)
	: theRequirement("")
{
	
}

gotoCondition::~gotoCondition()
{
}

void gotoCondition::setValue(std::string i)
{
	theRequirement = i;
}

void gotoCondition::Update(double dt)
{
	std::string temp;
	temp = Scene_System::accessing().getCurrScene().getName();
	//stuff to increase the number, please remember to increase the counter as you go along.
	if (!complete)
	{
		if (temp == theRequirement)
		{
			complete = true;
		}
	}
	else
	{
		return;
	}
}