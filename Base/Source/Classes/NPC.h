#ifndef NPC_H_
#define NPC_H_
#include "GameObject.h"
//#include "Dialouge.h"

class NPC : public GameObject
{
public:
	NPC();
	virtual ~NPC();

	bool getInteracting();

	virtual void Update(float);
private:
	//Dialouge* mySpeech;
	unsigned id;
	std::string name;
	bool interacting;
};
#endif