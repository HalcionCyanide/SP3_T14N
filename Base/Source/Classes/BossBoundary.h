#ifndef BOSSBOUNDARY_H
#define BOSSBOUNDARY_H

#include "GateBoundary.h"
#include "..//Systems/Scene_System.h"

class BossBoundary : public GateBoundary
{
public:
	BossBoundary();
	virtual ~BossBoundary();

	virtual float GetOverlappingDistance()const;
	virtual bool CheckCollision(const Vector3 &point);
};


#endif