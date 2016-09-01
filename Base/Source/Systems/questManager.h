/****************************************************************************/
/*!
\file NPC.h
\author Zheng Qingping
\par email: 152063Q@mymail.nyp.edu.sg
\brief
Container to store all quests
*/
/****************************************************************************/
#pragma once
#include "..\\Quests\\Quest.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

class questManager
{
public:
	questManager();
	virtual ~questManager();

	virtual void readFile(const std::string);
	virtual void Update(double);

	std::vector<Quest*> allQuests;
};