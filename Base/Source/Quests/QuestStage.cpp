#include "QuestStage.h"
#include "..\\Systems\\Scene_System.h"

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
		condition = new collectionCondition(value);
	}
	else if (c.find("rating") != std::string::npos)
	{
		condition = new ratingCondition(value);
	}
	else if (c.find("goto") != std::string::npos)
	{
		condition = new gotoCondition(value);
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
	{
		complete = condition->complete;
		condition->Update(dt);
	}
}