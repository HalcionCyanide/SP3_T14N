#ifndef goto_CONDITION_H_
#define goto_CONDITION_H_
#include "genericCondition.h"
#include "Vector3.h"

class gotoCondition : public genericCondition
{
public:
	gotoCondition();
	gotoCondition(std::string);
	virtual ~gotoCondition();

	virtual void setValue(std::string);
	virtual void Update(double dt);
private:
	std::string theRequirement;
};

#endif