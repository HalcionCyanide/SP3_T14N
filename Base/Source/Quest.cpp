#include "Quest.h"

Quest::Quest()
{
	id++;
	questName = "";
	currentStage = 0;
	active = false;
}

Quest::~Quest()
{
}

int Quest::getID()
{
	return id;
}

std::string Quest::getName()
{
	return questName;
}

void Quest::setName(std::string sample)
{
	questName = sample;
}

int Quest::getStage()
{
	return currentStage;
}

int Quest::getMaxStages()
{
	return questDesc.size();
}

void Quest::nextStage()
{
	if (active)
	{
		if (currentStage < getMaxStages())
		{
			currentStage++;
		}
		else
		{
			active = false;
			completed = true;
		}
	}
}

std::vector<std::string>& Quest::getDesc()
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
	os << quest.questName << "\n";

	os << quest.questDesc.at(quest.currentStage) << "\n";

	return os;
}