/****************************************************************************/
/*!
\file NPC.h
\author Zheng Qingping
\par email: 152063Q@mymail.nyp.edu.sg
\brief
Defines all flags for an NPC
*/
/****************************************************************************/
#ifndef NPC_H_
#define NPC_H_
#include "..\\Classes\\GameObject.h"
#include <map>

class NPC : public GameObject
{
public:
	NPC();
	virtual ~NPC();

	std::string getFText();
	virtual void setFText(std::string);
	bool getInteracting();
	virtual void setInteracting(bool);
	virtual void setTarget(const Vector3&);
	virtual Vector3 getTarget();

	virtual void SetDetectionRadiusSquared(const float&);
	virtual float GetDetectionRadiusSquared();

	virtual void Update(double dt);

	std::map < std::string, std::vector<int> > NPCcurrQstate;
private:
	float DetectionRadiusSquared;
	float currentAngle;
	bool interacting;
	std::string flavourText;
	Vector3 target; //AKA player's position.
};

#endif