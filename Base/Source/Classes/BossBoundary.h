/****************************************************************************/
/*!
\file BossBoundary.h
\author Tan Teck Ling
\par email: 153782U@mymail.nyp.edu.sg
\brief
Defines the basic values of a boundary
*/
/****************************************************************************/

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
	void SetEnemyID(const std::string &);

	std::string EnemyID;
	bool EnemyKilled = false;
};


#endif