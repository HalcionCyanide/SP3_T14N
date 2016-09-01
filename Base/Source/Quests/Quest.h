#pragma once
#include "..\\Classes\\genericEntity.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "QuestStage.h"

class Quest : public GenericEntity
{
public:
	Quest();
	virtual ~Quest();

	void LoadFile(std::string&);

	int getCurrentStage();
	void setCurrStage(int);

	int getID();
	void setID(int);

	bool getActive();
	void setActive(bool);

	void Update(double);
	QuestStage* theStageAT;
	std::vector<QuestStage*> qStages;
	std::string preReq;
	int preReqVal;
private:
	Item* I_Heal;
	Item* I_Accel;
	bool active;
	int qID;
};

