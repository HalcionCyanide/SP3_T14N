#ifndef NPC_GUARDCAPT_H_
#define NPC_GUARDCAPT_H_

#include <list>
#include "GenericNPC.h"
#include "Vector3.h"

class GuardCapt : public GenericNPC
{
public:
	GuardCapt();
	virtual ~GuardCapt();

	virtual void Init();
	virtual void Update(double dt);

	std::list<Quest*> available;
};

#endif