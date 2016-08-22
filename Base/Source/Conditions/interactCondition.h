#ifndef interact_CONDITION_H_
#define interact_CONDITION_H_
#include "genericCondition.h"
#include "..\\Classes\\GameObject.h"
#include "..\\Classes\\NPC.h"

class interactCondition : public genericCondition
{
public:
	interactCondition();
	interactCondition(NPC*);
	virtual ~interactCondition();

	virtual void Update(double dt);
private:
	Data<NPC*> theRequirement;
};

#endif