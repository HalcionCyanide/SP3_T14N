#ifndef capture_CONDITION_H_
#define capture_CONDITION_H_
#include "genericCondition.h"
#include "..\\Classes\\GameObject.h"

class captureCondition : public genericCondition
{
public:
	captureCondition();
	//captureCondition(Monster* i);
	virtual ~captureCondition();

	virtual void setValue(std::string);
	virtual void Update(double dt);
private:
	//Data<Monster*> theRequirement;
};

#endif