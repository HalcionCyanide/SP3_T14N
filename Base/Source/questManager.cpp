#include "questManager.h"

questManager::questManager()
{
}


questManager::~questManager()
{
}

void questManager::printQuestwithID(const int i)
{
	if (allQuests[i - 1].getActive())
	{
		if (i != 0)
		{
			std::cout << allQuests[i - 1] << std::endl;
		}
	}
}

void questManager::print()
{
	int i = 0;
	for (std::vector<Quest>::iterator it = allQuests.begin(); it < allQuests.end(); it++)
	{
		if (allQuests[i].getActive())
		{
			std::cout << allQuests[i] << std::endl;
			i++;
		}
	}
}

void questManager::readFile(const std::string fileName)
{
	std::string temp;

	std::ifstream file(fileName.c_str());

	if (file.is_open())
	{
		while (file.good())
		{
			std::getline(file, temp);
			tempQuest.setName(temp);
			temp.clear();
			allQuests.push_back(tempQuest);
		}
	}
}

void questManager::readDescFile(const std::string fileName)
{
	std::string temp;
	int currQuestAt = 0;
	
	std::ifstream fileStream(fileName, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "Impossible to open " << fileName << ". Are you in the right directory ?\n";
	}
	while (fileStream.good())
	{
		std::string temp;
		if (std::getline(fileStream, temp))
		{
			if (temp[0] == '[')
			{
				temp.clear();
				std::getline(fileStream, temp);
				allQuests.at(currQuestAt).getDesc().push_back(temp);
			}
			else if (temp[0] == ']')
			{
				currQuestAt++;
			}
			else
			{
				allQuests.at(currQuestAt).getDesc().push_back(temp);
			}
			temp.clear();
		}
	}
}