#ifndef capture_CONDITION_H_
#define capture_CONDITION_H_
#include "genericCondition.h"
#include "..\\Classes\\Enemy.h"

class captureCondition : public genericCondition
{
public:
	captureCondition();
	//captureCondition(std::string i);
	virtual ~captureCondition();

	virtual void Update(double dt);
private:
	Data<Enemy*> theRequirement;
};

#endif