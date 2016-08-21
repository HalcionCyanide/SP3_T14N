#include "Quest.h"
#include "collectionCondition.h"

Quest::Quest() : GenericEntity()
{
	id = 0;
	name_ = "";
	questDesc = "";
	active = false;
}

Quest::~Quest()
{
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
	/*<!>*/if (i == "collection")
	{
		conditions.push_back(new collectionCondition(std::stoi(value)));
	}
	/*else if (i.find("RATING") != std::string::npos)
	{
		conditions.push_back(new ratingCondition());
	}
	else if (i.find("INTERACT") != std::string::npos)
	{
		conditions.push_back(new interactCondition());
	}
	else if (i.find("GOTO") != std::string::npos)
	{
		conditions.push_back(new gotoCondition());
	}
	else if (i.find("CAPTURE") != std::string::npos)
	{
		conditions.push_back(new captureCondition());
	}*/
}


std::ostream& operator<<(std::ostream& os, const Quest& quest)
{
	os << quest.name_ << "\n" << quest.questDesc << "\n";
	return os;
}