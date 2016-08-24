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
	virtual void setTarget(const Vector3& i){ target = i; };
	virtual Vector3 getTarget(){ return target; };

	bool interacting;
	std::string flavourText;
	std::vector<Quest*> allQuests;
	std::vector<Quest*> completed;
private:
	Vector3 target;
};

#endif