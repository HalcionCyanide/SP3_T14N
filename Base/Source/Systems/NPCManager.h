#pragma once
#include "..\\Classes\\NPC.h"
#include <fstream>
#include <sstream>
#include <vector>

class NPCManager
{
public:
	NPCManager();
	~NPCManager();

	void readFile(const std::string);

	std::vector<NPC> allNPC;
	NPC tempQuest;
};