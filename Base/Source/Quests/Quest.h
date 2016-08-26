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

	QuestStage* getCurrentStage();
	void setCurrStage(int);

	int getID();
	void setID(int);

	void Update(double);
private:
	int qID;
	QuestStage* theStageAT;
	std::vector<QuestStage*> qStages;
};

