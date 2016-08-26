#include "QuestStage.h"

QuestStage::QuestStage()
{
	stageNO = 0;
	condition = nullptr;
	questDesc = "";
	complete = false;
	giver = "";
}

QuestStage::~QuestStage()
{
	if (condition)
		delete condition;
	condition = nullptr;
}

bool QuestStage::getComplete()
{
	return complete;
}

void QuestStage::setComplete(bool i)
{
	complete = i;
}

genericCondition* QuestStage::getConds()
{
	return condition;
}

void QuestStage::setStageNO(int i)
{
	stageNO = i;
}

int QuestStage::getStageNO()
{
	return stageNO;
}

void QuestStage::setCondition(std::string c, std::string value)
{
	if (c.find("collection") != std::string::npos)
	{
		condition = new collectionCondition(std::stoi(value));
	}
	else if (c.find("rating") != std::string::npos)
	{
		condition = new ratingCondition(std::stoi(value));
	}
	else if (c.find("interact") != std::string::npos)
	{
	//PASS IN NPC NAME
		condition = new interactCondition(value);
	}
	else if (c.find("goto") != std::string::npos)
	{
		//SPLIT THE STRING FROM X:X:X to a VECTOR3
		//SET THE CONDITION.
		condition = new gotoCondition(value);
	}
	else if (c.find("capture") != std::string::npos)
	{
	//PASS IN MONSTER NAME
	//condition = new captureCondition(value);
	}
}

std::string QuestStage::getDesc()
{
	return questDesc;
}

void QuestStage::setDesc(std::string i)
{
	questDesc = i;
}

std::string QuestStage::getGiver()
{
	return giver;
}

void QuestStage::setGiver(std::string i)
{
	giver = i;
}

void QuestStage::Update(double dt)
{
	if (!complete)
	condition->Update(dt);
}