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

void NPC::setID(int id)
{
	this->id = id;
}

void NPC::setFlavourText(std::string i)
{
	flavourText = i;
}

bool NPC::getInteracting()
{
	return interacting;
}

void NPC::Update(float dt)
{
}