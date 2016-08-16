#include "Billboard.h"

bool Billboard::CheckLife()
{
	// Still active if Liftime exceeds.
	if (LifeTime == -1) 
		return true;
	return CurrentTime < LifeTime;
}

void Billboard::SetType(bool IsParticle)
{
	this->IsParticle = IsParticle;
}

bool Billboard::operator<(Billboard& rhs){
	float V1 = (this->Position - this->PlayerPosition).LengthSquared();
	float V2 = (rhs.Position - this->PlayerPosition).LengthSquared();
	return (V1 > V2);
}