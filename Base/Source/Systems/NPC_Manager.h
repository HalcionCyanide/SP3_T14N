/****************************************************************************/
/*!
\file NPC_Manager.h
\author Zheng Qing Ping
\par email: 152063Q@mymail.nyp.edu.sg
\brief
Defines the a manager for a generic NPC
*/
/****************************************************************************/


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