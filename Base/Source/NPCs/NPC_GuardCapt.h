#ifndef NPC_GUARDCAPT_H_
#define NPC_GUARDCAPT_H_

#include <vector>
#include "GenericNPC.h"
#include "Vector3.h"

class GuardCapt : public GenericNPC
{
public:
	GuardCapt();
	virtual ~GuardCapt();

	virtual void Init();
	virtual void Update(double dt);	
};

#endif