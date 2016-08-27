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

void questManager::readFile(const std::string fileName)
{
	std::string temp = "";
	Quest *tempQuest = nullptr;
	std::string tempReq;

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
				tempReq = theValues[pos];

				it = std::find(theKeys.begin(), theKeys.end(), "PREREQVAL");
				pos = it - theKeys.begin();
				qPreReq.insert(std::pair<std::string, int>(tempReq, stoi(theValues[pos])));

				allQuests.push_back(tempQuest);
				theValues.clear();
			}
        }
	}
}

void questManager::Update(double dt)
{
	for (auto it : allQuests)
	{
		it->Update(dt);
	}
}