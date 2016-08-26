#ifndef QUESTSTAGE_H_
#define QUESTSTAGE_H_
#include "..\\Conditions\\genericCondition.h"

#include "..\\Conditions\\captureCondition.h"
#include "..\\Conditions\\collectionCondition.h"
#include "..\\Conditions\\gotoCondition.h"
#include "..\\Conditions\\interactCondition.h"
#include "..\\Conditions\\ratingCondition.h"
class QuestStage
{
public:
	QuestStage();
	virtual ~QuestStage();

	virtual void setDesc(std::string);
	std::string getDesc();

	genericCondition* getConds();
	virtual void setCondition(std::string, std::string);

	bool getComplete();
	virtual void setComplete(bool);

	int getStageNO();
	virtual void setStageNO(int);

	std::string getGiver();
	virtual void setGiver(std::string);

	virtual void Update(double dt);
private:
	int stageNO;
	genericCondition* condition;
	std::string questDesc;
	std::string giver;
	bool complete;
};

#endif