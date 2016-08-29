#include "Quest.h"

Quest::Quest()
	:qID(0)
{
	theStageAT = nullptr;
	active = false;
}

Quest::~Quest()
{
	for (auto it : qStages)
	{
		delete it;
	}
	qStages.clear();
}

struct to_upper
{
	int operator() (int ch)
	{
		return std::toupper(ch);
	}
};

int Quest::getCurrentStage()
{
	int i = 0;
	if (active)
	{
		for (auto it : qStages)
		{
			i++;
			if (it == theStageAT)
			{
				break;
			}
		}
	}
	return i;
}

void Quest::setCurrStage(int i)
{
	theStageAT = qStages.at(i);
}

int Quest::getID()
{
	return qID;
}

void Quest::setID(int i)
{
	qID = i;
}

void Quest::setActive(bool i)
{
	active = i;
}

bool Quest::getActive()
{
	return active;
}

void Quest::LoadFile(std::string& fileName)
{
	std::string temp = "";
	std::string tempCName = "";

	QuestStage* tempQS = nullptr;

	std::ifstream file2(fileName);
	if (file2.is_open())
	{
		std::vector<std::string> theKeys;
		std::vector<std::string> theValues;
		unsigned tempID = 0;
		while (std::getline(file2, temp))
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
				tempQS = new QuestStage();

				std::vector<std::string>::iterator it;

				it = std::find(theKeys.begin(), theKeys.end(), "STAGE");
				size_t pos = it - theKeys.begin();
				tempQS->setStageNO(stoi(theValues[pos]));

				it = std::find(theKeys.begin(), theKeys.end(), "CONDITIONNAME");
				pos = it - theKeys.begin();
				tempCName = theValues[pos];

				it = std::find(theKeys.begin(), theKeys.end(), "VALUE");
				pos = it - theKeys.begin();
				tempQS->setCondition(tempCName, theValues[pos]);

				it = std::find(theKeys.begin(), theKeys.end(), "DESCRIPTION");
				pos = it - theKeys.begin();
				tempQS->setDesc(theValues[pos]);

				it = std::find(theKeys.begin(), theKeys.end(), "QORIGIN");
				pos = it - theKeys.begin();
				tempQS->setGiver(theValues[pos]);

				qStages.push_back(tempQS);
				theValues.clear();
			}
		}
	}
}

void Quest::Update(double dt)
{
	if (theStageAT != nullptr)
	{
		theStageAT->Update(dt);
	}
	else
	{
		active = false;
	}
}