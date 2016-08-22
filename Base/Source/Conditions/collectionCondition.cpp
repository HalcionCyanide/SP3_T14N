#include "collectionCondition.h"


collectionCondition::collectionCondition()
    : theRequirement(0)
	, counter(0)
{
	//target = nullptr;
}

collectionCondition::collectionCondition(int i)
	: theRequirement(0)
	, counter(0)
{
	theRequirement = i;
}

collectionCondition::~collectionCondition()
{
}

void collectionCondition::setValue(std::string i)
{
	theRequirement = stoi(i);
}

void collectionCondition::Update(double dt)
{
	//stuff to increase the number, please remember to increase the counter as you go along.

	/*
	counter = capturedMonsters.size(); //eventually a vector for captured monsters. <?>
	if(!satisfied)
	{
		if(theRequirement <= counter)
			satisfied = true;
	}
	else
	{
		return;
	}
	*/
}