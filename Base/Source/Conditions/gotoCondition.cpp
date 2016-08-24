#include "gotoCondition.h"
#include <sstream>

gotoCondition::gotoCondition()
	: theRequirement(Vector3(0,0,0))
	, dist(0)
{
}

gotoCondition::gotoCondition(std::string i)
	: theRequirement(Vector3(0, 0, 0))
	, dist(0)
{
	unsigned short num = 0;
	std::istringstream iss(i);
	std::string data = "";
	//Tikam Tikam write it. Pray it works.
	while (getline(iss, data, ';'))
	{
		if (num == 0)
				theRequirement.value_.x = stof(data);
		else if (num == 1)
			theRequirement.value_.y = stof(data);
		else if (num == 2)
			theRequirement.value_.z = stof(data);
		++num;
	}
}

gotoCondition::~gotoCondition()
{
}

void gotoCondition::setValue(std::string i)
{
	unsigned short num = 0;
	std::istringstream iss(i);
	std::string data = "";
	//Tikam Tikam write it. Pray it works.
	while (getline(iss, data, ':'))
	{
		if (num == 0)
			theRequirement.value_.x = stof(data);
		else if (num == 1)
			theRequirement.value_.y = stof(data);
		else if (num == 2)
			theRequirement.value_.z = stof(data);
		++num;
	}
}

void gotoCondition::Update(double dt)
{
	//stuff to increase the number, please remember to increase the counter as you go along.
	if (!complete)
	{
		//if ((Player.pos - (*theRequirement.value_)).LengthSquared() < dist)
		{
			complete = true;
		}
	}
	else
	{
		return;
	}
}