#include "NPC.h"

NPC::NPC()
	:	id(0)
	, name("")
	, interacting(false)
{

}

NPC::~NPC()
{

}

bool NPC::getInteracting()
{
	return interacting;
}

void NPC::Update(float dt)
{
}