#include "questManager.h"

questManager::questManager()
{
}


questManager::~questManager()
{

}

struct to_upper
{
	int operator() (int ch)
	{
		return std::toupper(ch);
	}
};

bool sortByName(Quest* first, Quest* second)
{
	return (first->getName() < second->getName());
}

void questManager::readFile(const std::string fileName)
{
	std::string temp = "";
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

				it = std::find(theKeys.begin(), theKeys.end(), "PREREQ");
				pos = it - theKeys.begin();
				tempQuest->preReq = theValues[pos];

				it = std::find(theKeys.begin(), theKeys.end(), "PREREQVAL");
				pos = it - theKeys.begin();
				tempQuest->preReqVal = stoi(theValues[pos]);

				std::string path = "DrivenFiles//quests//q_";
				std::string fileFormat = ".csv";
				std::string temp2 = path + tempQuest->getName() + fileFormat;
				tempQuest->LoadFile(temp2);
				tempQuest->setCurrStage(0);

				allQuests.push_back(tempQuest);
				theValues.clear();
			}
        }
	}
	std::sort(allQuests.begin(), allQuests.end(), sortByName);
}

void questManager::Update(double dt)
{
	for (auto it : allQuests)
	{
		it->Update(dt);
	}
}