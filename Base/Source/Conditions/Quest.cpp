#include "Quest.h"
#include "collectionCondition.h"
#include "ratingCondition.h"

Quest::Quest() : GenericEntity()
{
	id = 0;
	name_ = "";
	questDesc = "";
	active = false;
}

Quest::~Quest()
{
	for (auto it : conditions)
	{
		delete it;
	}
	conditions.clear();
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

void Quest::setCondition(std::string i, std::string value)
{
	if (i.find("collection") != std::string::npos)
	{
		conditions.push_back(new collectionCondition(std::stoi(value)));
	}
	if (i.find("rating") != std::string::npos)
	{
		conditions.push_back(new ratingCondition(std::stoi(value)));
	}
	/*else if (i.find("interact") != std::string::npos)
	{
		conditions.push_back(new interactCondition());
	}
	else if (i.find("goto") != std::string::npos)
	{
		conditions.push_back(new gotoCondition());
	}
	else if (i.find("capture") != std::string::npos)
	{
		conditions.push_back(new captureCondition());
	}*/
}


std::ostream& operator<<(std::ostream& os, const Quest& quest)
{
	os << quest.name_ << "\n" << quest.questDesc << "\n";
	return os;
}