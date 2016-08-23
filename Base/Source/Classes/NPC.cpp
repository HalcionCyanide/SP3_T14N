#include "NPC.h"
#include "..\\Systems\\Scene_System.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

struct to_upper
{
	int operator() (int ch)
	{
		return std::toupper(ch);
	}
};

NPC::NPC()
	:	id(0)
	, interacting(false)
{
	std::string temp = "";
	int tempID = 0;

	std::ifstream file("..\\Conditions\\questData.csv");
	if (file.is_open())
	{
		std::vector<std::string> theKeys;
		std::vector<std::string> theValues;
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

				std::vector<std::string>::iterator it;

				it = std::find(theKeys.begin(), theKeys.end(), "ID");
				size_t pos = it - theKeys.begin();
				tempID = stoi(theValues[pos]);

				it = std::find(theKeys.begin(), theKeys.end(), "VALUE");
				pos = it - theKeys.begin();
				if (name_ == theValues[pos])
				{
					myQuests.push_back(Scene_System::accessing().QM.allQuests.at(tempID - 1));
				}
				theValues.clear();
			}

		}
	}
}

NPC::~NPC()
{
}

void NPC::setID(int id)
{
	this->id = id;
}

void NPC::setFlavourText(std::string i)
{
	flavourText = i;
}

bool NPC::getInteracting()
{
	return interacting;
}

void NPC::Update(float dt)
{

}