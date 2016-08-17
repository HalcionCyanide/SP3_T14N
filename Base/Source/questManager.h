#pragma once
#include "Quest.h"
#include <fstream>
#include <sstream>
class questManager
{
public:
	questManager();
	virtual ~questManager();

	virtual void readFile(const std::string);
	virtual void readDescFile(const std::string);
	virtual void print();
	virtual void printQuestwithID(const int);

	std::vector<Quest> allQuests;
	Quest tempQuest;
};

