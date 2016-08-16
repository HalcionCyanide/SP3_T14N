#pragma once
#include "Quest.h"
#include "GenericSystem.h"
#include <fstream>
#include <sstream>
class questManager : public GenericSystem
{
public:
	questManager();
	virtual ~questManager();

	virtual void readFile(const std::string);
	virtual void readDescFile(const std::string);
	virtual void print();
	virtual void printQuestwithID(int ID);

	std::vector<Quest> allQuests;
	Quest tempQuest;
};

