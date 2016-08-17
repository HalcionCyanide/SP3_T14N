#ifndef _UI_SYSTEM_H
#define _UI_SYSTEM_H

#include "GenericSystem.h"

class UI_System : public GenericSystem
{
public:
	UI_System();
	virtual ~UI_System();

	virtual void Init();
	virtual void Update(float dt);
	virtual void Render();
	virtual void Exit();

private:

};

#endif // _UI_SYSTEM_H