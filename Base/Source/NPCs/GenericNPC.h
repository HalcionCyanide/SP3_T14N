#ifndef GENERIC_NPC_H_
#define GENERIC_NPC_H_
#include <string>
#include "..\\Classes\\GameObject.h"
#include "..\\Conditions\\Quest.h"
#include <vector>

struct GenericNPC : public GameObject
{
	GenericNPC(){ interacting = false; flavourText = ""; };
	virtual ~GenericNPC(){};
	
	virtual void Update(double dt) = 0;

	bool interacting;
	std::string flavourText;
	std::vector<Quest*> myQuests;
};

#endif