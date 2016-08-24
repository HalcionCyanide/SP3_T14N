#include "Quest.h"
#include "collectionCondition.h"
#include "ratingCondition.h"
#include "interactCondition.h"
#include "captureCondition.h"
#include "gotoCondition.h"

Quest::Quest() : GenericEntity()
{
	id = 0;
	name_ = "";
	questDesc = "";
	active = false;
	complete = false;
	conditions = nullptr;
}

Quest::~Quest()
{
	if (conditions)
	{
		delete conditions;
	}
}

void Quest::setID(int i)
{
	id = i;
}

int Quest::getID()
{
	return id;
}

void Quest::setDesc(std::string i)
{
	questDesc = i;
}

std::string Quest::getDesc()
{
	return questDesc;
}

bool Quest::getActive()
{
	return active;
}

void Quest::setActive(bool temp)
{
	active = temp;
}

genericCondition* Quest::getConds()
{
	return conditions;
}

void Quest::setCondition(std::string i, std::string value)
{
	if (i.find("collection") != std::string::npos)
	{
		conditions = new collectionCondition(std::stoi(value));
	}
	else if (i.find("rating") != std::string::npos)
	{
		conditions = new ratingCondition(std::stoi(value));
	}
	/*else if (i.find("interact") != std::string::npos)
	{
		//PASS IN NPC NAME

		conditions.push_back(new interactCondition(value));
	}*/
	else if (i.find("goto") != std::string::npos)
	{
		//SPLIT THE STRING FROM X:X:X to a VECTOR3
		//SET THE CONDITION.
	
		conditions = new gotoCondition(value);
	}
	/*
	else if (i.find("capture") != std::string::npos)
	{
		//PASS IN MONSTER NAME

		conditions.push_back(new captureCondition());
	}*/
}


std::ostream& operator<<(std::ostream& os, const Quest& quest)
{
	os << quest.name_ << "\n" << quest.questDesc << "\n";
	return os;
}

void Quest::Update(double dt)
{
	complete = conditions->complete;
}