#include "captureCondition.h"

captureCondition::captureCondition()
	//: theRequirement(nullptr)
{
}

//captureCondition::captureCondition(Monster* i)
//	: theRequirement(i)
//{
//}

captureCondition::~captureCondition()
{
}

void captureCondition::Update(double dt)
{
	//stuff to increase the number, please remember to increase the counter as you go along.

	if(!satisfied)
	{
		//if ((theRequirement.value_->captured))
		satisfied = true;
	}
	else
	{
	return;
	}
}