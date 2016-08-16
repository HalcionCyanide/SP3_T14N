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
		try
		{
			if (currentStage < getMaxStages())
			{
				currentStage++;
			}
			else
			{
				throw 0;
			}
		}
		catch (int a)
		{
			if (a == 0)
			{
				std::cout << "Max Quest Stage detected! Ignoring input, reverting to last known valid stage." << std::endl;
			}
		}
	}
}

void Quest::setStage(int stage)
{
	if (active)
	{
		try
		{
			if (stage < getMaxStages())
			{
				currentStage = stage;
			}
			else
			{
				throw 0;
			}
		}
		catch (int a)
		{
			if (a == 0)
			{
				std::cout << "Invalid quest Stage detected! Ignoring input, reverting to last known valid stage." << std::endl;
			}
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

std::ostream& operator<<(std::ostream& os, const Quest& quest)
{
	if (quest.active)
	{
		Quest tempQuest = quest;

		os << "ID #" << quest.id << " " << quest.questName << "\n";

		os << quest.questDesc.at(quest.currentStage) << "\n";

		return os;
	}
	return os;
}