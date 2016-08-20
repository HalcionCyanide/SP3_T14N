#include "Quest.h"

Quest::Quest()
{
	id++;
	questName = "";
	active = false;
	completed = false;
}

Quest::~Quest()
{
}

int Quest::getID()
{
	return id;
}

void Quest::setName(std::string i)
{
	questName = i;
}

std::string Quest::getName()
{
	return questName;
}

void Quest::setDesc(std::string i)
{
	questDesc = 1;
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

bool Quest::getComplete()
{
	return completed;
}

void Quest::setComplete(bool temp)
{
	completed = temp;
}

std::ostream& operator<<(std::ostream& os, const Quest& quest)
{
	os << quest.questName << "\n" << quest.questDesc << "\n";
	return os;
}