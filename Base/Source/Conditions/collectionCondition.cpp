#include "collectionCondition.h"
#include "..\\Systems\\Scene_System.h"

collectionCondition::collectionCondition()
    : theRequirement("")
	, counter(0)
{
	//target = nullptr;
}

collectionCondition::collectionCondition(std::string i)
	: theRequirement(i)
	, counter(0)
{
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
	if(!complete)
	{
		counter = Scene_System::accessing().gPlayer->getMonsterCount();
		if (counter >= stoi(theRequirement))
			complete = true;
	}
	else
	{
		return;
	}
}