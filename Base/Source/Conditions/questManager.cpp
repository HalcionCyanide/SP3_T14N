#include "questManager.h"

questManager::questManager()
{
}


questManager::~questManager()
{
}

void questManager::readFile(const std::string fileName)
{
	std::string temp;

	std::ifstream file(fileName.c_str());

	if (file.is_open())
	{
		while (file.good())
		{
			std::getline(file, temp,',');
			tempQuest.setName(temp);
			temp.clear();

			std::getline(file, temp, ',');
			temp.clear();

			allQuests.push_back(tempQuest);
		}
	}
}