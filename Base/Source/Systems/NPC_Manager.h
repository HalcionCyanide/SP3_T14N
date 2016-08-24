#ifndef GENERIC_NPC_H_
#define GENERIC_NPC_H_
#include <vector>
#include "..\\NPCs\\NPC.h"
#include <fstream>
#include <sstream>

class NPCManager
{
public:
	NPCManager();
	virtual ~NPCManager();
	
	virtual void LoadFile(const std::string& fileName);
	std::vector<NPC*> allNPCs;
};

#endif