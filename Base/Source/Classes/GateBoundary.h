#ifndef GATEBOUNDARY_H
#define GATEBOUNDARY_H

#include "Boundary.h"
#include "..//Systems/Scene_System.h"

class GateBoundary : public Boundary
{
public:
	GateBoundary();
	virtual ~GateBoundary();
	
	virtual float GetOverlappingDistance()const;
	virtual bool CheckCollision(const Vector3 &point);
	virtual void TransitScene(const std::string &ID);

	void SetQuestData(const std::string &,const int &);
	
private:
	void InitQuest();

	Quest* questToCheck;
	QuestStage* stageToCheck;
	int questStage;
	std::string questName;
};


#endif