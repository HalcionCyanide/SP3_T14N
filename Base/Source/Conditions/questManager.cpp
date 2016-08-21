#include "questManager.h"
#include <algorithm>
#include <cctype>

questManager::questManager()
{
}


questManager::~questManager()
{
	for (auto it : allQuests)
	{
		delete it;
	}
	allQuests.clear();
}

struct to_upper
{
	int operator() (int ch)
	{
		return std::toupper(ch);
	}
};

void questManager::readFile(const std::string fileName)
{
	std::string temp = "";
	std::string tempName = "";
	std::string tempValue = "";
	Quest *tempQuest = nullptr;

	std::ifstream file(fileName.c_str());
	if (file.is_open())
	{
		std::vector<std::string> theKeys;
		std::vector<std::string> theValues;
		while (std::getline(file,temp))
		{
			if (temp == "" || temp == "\n" || temp == "\r")
				continue;
			std::string token;
			std::istringstream iss(temp);
			if (theKeys.empty())
			{   //Get the keys from CSV
				while (std::getline(iss, token, ','))
				{
					std::transform(token.begin(), token.end(), token.begin(), to_upper());
					theKeys.push_back(token);
				}
			}
			else
			{  //Begin getting all the values from the CSV
				while (std::getline(iss, token, ','))
				{
					theValues.push_back(token);
				}
				tempQuest = new Quest();

				std::vector<std::string>::iterator it;

				it = std::find(theKeys.begin(), theKeys.end(), "ID");
				size_t pos = it - theKeys.begin();
				tempQuest->setID(stoi(theValues[pos]));

				it = std::find(theKeys.begin(), theKeys.end(), "NAME");
				pos = it - theKeys.begin();
				tempQuest->setName(theValues[pos]);

				it = std::find(theKeys.begin(), theKeys.end(), "CONDITIONNAME");
				pos = it - theKeys.begin();
				tempName = theValues[pos];

				it = std::find(theKeys.begin(), theKeys.end(), "VALUE");
				pos = it - theKeys.begin();
				tempValue = theValues[pos];
				tempQuest->setCondition(tempName, tempValue);
				tempName.clear();
				tempValue.clear();

				it = std::find(theKeys.begin(), theKeys.end(), "DESCRIPTION");
				pos = it - theKeys.begin();
				tempQuest->setDesc(theValues[pos]);

				allQuests.push_back(tempQuest);
				theValues.clear();
			}

		}
	}
}