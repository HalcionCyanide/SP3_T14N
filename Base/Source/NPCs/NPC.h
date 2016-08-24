#ifndef NPC_H_
#define NPC_H_
#include "..\\Classes\\GameObject.h"

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

	virtual void Update(double dt);
private:
	float currentAngle;
	bool interacting;
	std::string flavourText;
	Vector3 target; //AKA player's position.
};

#endif