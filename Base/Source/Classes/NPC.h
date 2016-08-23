#ifndef NPC_H_
#define NPC_H_
#include "GameObject.h"
#include "..\\Conditions\\questManager.h"
//#include "Dialouge.h"

class NPC : public GameObject
{
public:
	NPC();
	virtual ~NPC();

	virtual void setID(int);

	virtual void setFlavourText(std::string);

	bool getInteracting();

	virtual void Update(float);
private:
	//Dialouge* mySpeech;
	unsigned id;
	std::string flavourText;
	bool interacting;
	std::vector<Quest*> myQuests;
};
#endif