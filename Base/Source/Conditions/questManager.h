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

	std::vector<Quest*> allQuests;

	Quest tempQuest;
};