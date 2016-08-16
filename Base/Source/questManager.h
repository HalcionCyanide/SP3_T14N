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
	void printQuestwithID(int ID);

	std::vector<Quest> allQuests;
	Quest tempQuest;
};

