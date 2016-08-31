#ifndef BOSSBOUNDARY_H
#define BOSSBOUNDARY_H

#include "Boundary.h"
#include "..//Systems/Scene_System.h"

class BossBoundary : public Boundary
{
public:
	BossBoundary();
	virtual ~BossBoundary();

	virtual float GetOverlappingDistance()const;
	virtual bool CheckCollision(const Vector3 &point);
};


#endif