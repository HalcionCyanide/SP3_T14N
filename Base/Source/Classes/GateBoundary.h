/****************************************************************************/
/*!
\file Boundary2D.h
\author Tan Teck Ling
\par email: 153782U@mymail.nyp.edu.sg
\brief
Inherits from the Boundary class for a custom Collision item.
*/
/****************************************************************************/
#ifndef GATEBOUNDARY_H
#define GATEBOUNDARY_H

#include "Boundary.h"
#include "..//Quests/Quest.h"

class GateBoundary : public Boundary
{
public:
	GateBoundary();
	virtual ~GateBoundary();
	
	virtual float GetOverlappingDistance()const;
	virtual bool CheckCollision(const Vector3 &point);
	virtual void TransitScene(const std::string &ID);

	void SetQuestData(const std::string &, const int &);
	void SetEnemyID(const std::string &);
	void SetEnemyKilled(const bool &);
	void SetBossGate(const bool &);
	void SetBossOnly(const bool &);

	std::string GetEnemyID();
	bool GetEnemyKilled();
	bool GetBossGate();

	virtual void Render();

	bool CheckQuest();

	std::string EnemyID;
private:
	void InitQuest();

	Quest* questToCheck;
	QuestStage* stageToCheck;
	int questStage;
	std::string questName;

	bool QuestCleared = false;
	bool BossGate = false;
	bool BossOnly = false;
};


#endif