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
	virtual void Update(double dt);

	std::vector<Quest*> allQuests;
	std::vector<Quest*> completedQuests;
};