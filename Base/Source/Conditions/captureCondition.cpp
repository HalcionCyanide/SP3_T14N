#include "captureCondition.h"

captureCondition::captureCondition()
	: theRequirement(nullptr)
{
}

//captureCondition::captureCondition(std::string i)
//	: theRequirement(i)
//{
//}

captureCondition::~captureCondition()
{
}

void captureCondition::Update(double dt)
{
	//stuff to increase the number, please remember to increase the counter as you go along.

	if(!complete)
	{
		complete = true;
	}
	else
	{
	return;
	}
}