#pragma once
#include "..\\Conditions\\Quest.h"
#include <fstream>
#include <sstream>
class questManager
{
public:
	questManager();
	virtual ~questManager();

	virtual void readFile(const std::string);

	std::vector<Quest*> allQuests;
	std::vector<Quest*> completedQuests;
};