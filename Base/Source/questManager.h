#pragma once
#include "Quest.h"
#include <fstream>
#include <sstream>
class questManager
{
public:
	questManager();
	~questManager();

	void readFile(const std::string);
	void readDescFile(const std::string);
	void print();
	void printQuestwithID(const int);

	std::vector<Quest> allQuests;
	std::vector<Quest> completedQuests;
	Quest tempQuest;
};