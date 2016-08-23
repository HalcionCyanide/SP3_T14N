#include "NPCManager.h"
#include <algorithm>
#include <cctype>

NPCManager::NPCManager()
{
}

NPCManager::~NPCManager()
{
	for (auto it : allNPCs)
	{
		delete it;
	}
	allNPCs.clear();
}

struct to_upper
{
	int operator() (int ch)
	{
		return std::toupper(ch);
	}
};

void NPCManager::readFile(const std::string fileName)
{
	std::string temp = "";
	std::string tempName = "";
	std::string tempValue = "";
	NPC *tempNPC = nullptr;

	std::ifstream file(fileName.c_str());
	if (file.is_open())
	{
		std::vector<std::string> theKeys;
		std::vector<std::string> theValues;
		unsigned tempID = 0;
		while (std::getline(file, temp))
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

				tempNPC = new NPC();

				std::vector<std::string>::iterator it;

				it = std::find(theKeys.begin(), theKeys.end(), "NAME");
				size_t pos = it - theKeys.begin();
				tempNPC->setName(theValues[pos]);

				it = std::find(theKeys.begin(), theKeys.end(), "TEXT");
				pos = it - theKeys.begin();
				tempNPC->setFlavourText(theValues[pos]);

				tempNPC->setID(tempID);
				allNPCs.push_back(tempNPC);
				theValues.clear();
				tempID++;
			}
		}
	}
}