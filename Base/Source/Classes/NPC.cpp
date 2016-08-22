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

bool NPC::getINTT()
{
	return interacting;
}

void NPC::Update(float dt)
{
}